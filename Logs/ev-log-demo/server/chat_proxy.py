from __future__ import annotations

import asyncio
import base64
import json
import os
import shutil
import subprocess
import tempfile
from pathlib import Path
from typing import Any
from urllib import error as urllib_error
from urllib import request as urllib_request

from pydantic import BaseModel, Field

LOCAL_PROVIDER_SPECS = {
    'codex_cli': {
        'label': 'Codex CLI',
        'command': 'codex',
    },
    'qwen_cli': {
        'label': 'Qwen Code',
        'command': 'qwen',
    },
    'iflow_cli': {
        'label': 'iFlow CLI',
        'command': 'iflow',
    },
}


class ChatSettings(BaseModel):
    providerType: str = 'codex_cli'
    baseUrl: str = ''
    apiKey: str = ''
    model: str = ''
    includeImage: bool = True


class ChatRequest(BaseModel):
    debugSessionId: str
    runId: str
    imageName: str | None = None
    message: str = ''
    settings: ChatSettings = Field(default_factory=ChatSettings)


def get_local_provider_statuses() -> dict[str, dict[str, Any]]:
    providers: dict[str, dict[str, Any]] = {}
    for key, spec in LOCAL_PROVIDER_SPECS.items():
        resolved_path = shutil.which(spec['command'])
        providers[key] = {
            'key': key,
            'label': spec['label'],
            'command': spec['command'],
            'available': bool(resolved_path),
            'path': resolved_path,
        }
    return providers


def safe_json_loads(raw_text: str) -> dict[str, Any]:
    try:
        payload = json.loads(raw_text) if raw_text else {}
    except json.JSONDecodeError:
        return {}

    return payload if isinstance(payload, dict) else {}


def normalize_base_url(base_url: str) -> str:
    trimmed = str(base_url or '').strip().rstrip('/')
    if not trimmed:
        return ''

    suffix = '/chat/completions'
    return trimmed[: -len(suffix)] if trimmed.endswith(suffix) else trimmed


def extract_assistant_text(message_content: Any) -> str:
    if isinstance(message_content, str):
        return message_content

    if isinstance(message_content, list):
        parts: list[str] = []
        for item in message_content:
            if isinstance(item, str):
                parts.append(item)
            elif isinstance(item, dict) and isinstance(item.get('text'), str):
                parts.append(item['text'])
        return '\n'.join(parts).strip()

    return ''


def build_image_data_url(image_path: Path | None) -> str | None:
    if not image_path or not image_path.is_file():
        return None

    payload = base64.b64encode(image_path.read_bytes()).decode('ascii')
    mime_type = 'image/png' if image_path.suffix.lower() == '.png' else 'application/octet-stream'
    return f'data:{mime_type};base64,{payload}'


def format_percent_text(value: float | None) -> str:
    return f'{value * 100:.1f}' if isinstance(value, (int, float)) else '无'


def build_run_summary_context(run: dict[str, Any]) -> str:
    summary = run.get('summary') or {}
    sim_info = run.get('simInfo') or {}
    settings_lines = '\n'.join(f'{key}: {value}' for key, value in sim_info.items()) if sim_info else '无'
    return '\n'.join(
        [
            f"当前仿真目录: {run.get('id') or '未知'}",
            f"采样点数量: {summary.get('sampleCount') if summary.get('sampleCount') is not None else '无'}",
            f"仿真时长(s): {summary.get('durationSeconds') if summary.get('durationSeconds') is not None else '无'}",
            f"检测覆盖率: {format_percent_text(summary.get('detectionCoverage'))}%",
            f"目标丢失比例: {format_percent_text(summary.get('lossRatio'))}%",
            f"目标锁定比例: {format_percent_text(summary.get('lockRatio'))}%",
            f"平均距离误差(m): {summary.get('avgAbsDistanceError') if summary.get('avgAbsDistanceError') is not None else '无'}",
            f"最大距离误差(m): {summary.get('maxAbsDistanceError') if summary.get('maxAbsDistanceError') is not None else '无'}",
            f"末端自车速度(m/s): {summary.get('egoFinalSpeed') if summary.get('egoFinalSpeed') is not None else '无'}",
            f"末端目标速度(m/s): {summary.get('targetFinalSpeed') if summary.get('targetFinalSpeed') is not None else '无'}",
            f"末端真值距离(m): {summary.get('finalGroundTruthDistance') if summary.get('finalGroundTruthDistance') is not None else '无'}",
            f"末端雷达距离(m): {summary.get('finalRadarDistance') if summary.get('finalRadarDistance') is not None else '无'}",
            f"末端相对速度(m/s): {summary.get('finalClosingSpeed') if summary.get('finalClosingSpeed') is not None else '无'}",
            f'仿真参数:\n{settings_lines}',
        ]
    )


def build_debug_session_context(debug_session: dict[str, Any]) -> str:
    entries = debug_session.get('entries') or []
    lines = [
        f"当前调试会话: {debug_session.get('title') or '未命名调试会话'}",
        f"调试目标: {debug_session.get('goal') or '尚未填写'}",
        f'已关联仿真次数: {len(entries)}',
    ]

    if not entries:
        lines.append('当前还没有加入任何仿真记录。')
        return '\n'.join(lines)

    lines.append('最近的调试过程:')
    for index, entry in enumerate(entries[-6:], start=max(1, len(entries) - 5)):
        run = entry.get('run') or {}
        summary = run.get('summary') or {}
        lines.extend(
            [
                f"{index}. Run {entry['runId']}",
                f"   更新时间: {run.get('updatedAt') or '未知'}",
                f"   检测覆盖率: {format_percent_text(summary.get('detectionCoverage'))}%",
                f"   目标丢失比例: {format_percent_text(summary.get('lossRatio'))}%",
                f"   平均距离误差: {summary.get('avgAbsDistanceError') if summary.get('avgAbsDistanceError') is not None else '无'} m",
                f"   调整说明: {entry.get('changeNote') or '未记录'}",
                f"   调整假设: {entry.get('hypothesis') or '未记录'}",
                f"   结果结论: {entry.get('resultNote') or '未记录'}",
            ]
        )
    return '\n'.join(lines)


def build_codex_prompt(
    *,
    debug_session: dict[str, Any],
    run: dict[str, Any],
    user_message: str,
) -> str:
    return '\n\n'.join(
        [
            '你是自动驾驶仿真调试助手。请始终使用简体中文回答，优先基于给定日志、图表和图片，不要编造不存在的数据。',
            '回答时请尽量按以下结构输出：1. 现象判断 2. 可能原因 3. 下一轮参数或排查建议。',
            '[调试会话上下文]\n' + build_debug_session_context(debug_session),
            '[当前聚焦的仿真日志]\n' + build_run_summary_context(run),
            f'[用户当前问题]\n{user_message.strip()}',
        ]
    )


def build_remote_messages(
    *,
    debug_session: dict[str, Any],
    run: dict[str, Any],
    user_message: str,
    image_data_url: str | None,
) -> list[dict[str, Any]]:
    messages: list[dict[str, Any]] = [
        {
            'role': 'system',
            'content': '你是自动驾驶仿真调试助手。请始终使用简体中文回答，优先基于给定日志、图表和图片，不要编造不存在的数据。',
        },
        {
            'role': 'system',
            'content': build_debug_session_context(debug_session),
        },
        {
            'role': 'system',
            'content': build_run_summary_context(run),
        },
    ]

    for item in debug_session.get('messages') or []:
        role = item.get('role')
        content = str(item.get('content') or '').strip()
        if role in {'system', 'assistant', 'user'} and content:
            messages.append({'role': role, 'content': content})

    if image_data_url:
        messages.append(
            {
                'role': 'user',
                'content': [
                    {'type': 'text', 'text': user_message.strip()},
                    {'type': 'image_url', 'image_url': {'url': image_data_url}},
                ],
            }
        )
    else:
        messages.append({'role': 'user', 'content': user_message.strip()})

    return messages


def parse_codex_json_output(stdout: str) -> tuple[str | None, str | None, dict[str, Any] | None]:
    thread_id: str | None = None
    message: str | None = None
    usage: dict[str, Any] | None = None

    for raw_line in stdout.splitlines():
        line = raw_line.strip()
        if not line:
            continue

        try:
            event = json.loads(line)
        except json.JSONDecodeError:
            continue

        if event.get('type') == 'thread.started':
            thread_id = event.get('thread_id') or thread_id
        elif event.get('type') == 'item.completed':
            item = event.get('item') or {}
            if item.get('type') == 'agent_message' and isinstance(item.get('text'), str):
                message = item['text'].strip() or message
        elif event.get('type') == 'turn.completed' and isinstance(event.get('usage'), dict):
            usage = event['usage']

    return thread_id, message, usage


def build_stateless_history_lines(debug_session: dict[str, Any]) -> str:
    messages = debug_session.get('messages') or []
    if not messages:
        return '暂无历史对话。'

    history_lines: list[str] = []
    for message in messages[-12:]:
        role = message.get('role')
        role_label = '用户' if role == 'user' else '助手' if role == 'assistant' else '系统'
        content = str(message.get('content') or '').strip()
        if content:
            history_lines.append(f'{role_label}: {content}')
    return '\n'.join(history_lines) if history_lines else '暂无历史对话。'


def build_qwen_prompt(
    *,
    debug_session: dict[str, Any],
    run: dict[str, Any],
    user_message: str,
    include_history: bool = True,
) -> str:
    sections = [
        '你是自动驾驶仿真调试助手。请始终使用简体中文回答，优先基于给定日志、图表和图片，不要编造不存在的数据。',
        '回答时请尽量按以下结构输出：1. 现象判断 2. 可能原因 3. 下一轮参数或排查建议。',
        '[调试会话上下文]\n' + build_debug_session_context(debug_session),
    ]

    if include_history:
        sections.append('[最近对话历史]\n' + build_stateless_history_lines(debug_session))

    sections.extend(
        [
            '[当前聚焦的仿真日志]\n' + build_run_summary_context(run),
            f'[用户当前问题]\n{user_message.strip()}',
        ]
    )
    return '\n\n'.join(sections)


def extract_iflow_message(stdout: str) -> str:
    if not stdout:
        return ''

    marker = '<Execution Info>'
    if marker in stdout:
        stdout = stdout.split(marker, 1)[0]

    lines: list[str] = []
    for raw_line in stdout.splitlines():
        line = raw_line.strip()
        if not line:
            continue
        if line.startswith('ℹ️') or line.startswith('Resuming session'):
            continue
        lines.append(line)

    return '\n'.join(lines).strip()


def parse_iflow_execution_info(*raw_texts: str) -> dict[str, Any]:
    marker_start = '<Execution Info>'
    marker_end = '</Execution Info>'

    for raw_text in raw_texts:
        text = str(raw_text or '')
        if not text or marker_start not in text:
            continue

        _, remainder = text.split(marker_start, 1)
        if marker_end in remainder:
            candidate = remainder.split(marker_end, 1)[0].strip()
        else:
            candidate = remainder.strip()

        payload = safe_json_loads(candidate)
        if payload:
            return payload

    return {}


def normalize_token_usage(
    usage: dict[str, Any] | None,
    *,
    prompt_key: str,
    completion_key: str,
    total_key: str,
    cached_key: str | None = None,
) -> dict[str, Any] | None:
    if not isinstance(usage, dict):
        return None

    normalized = {
        'prompt_tokens': usage.get(prompt_key),
        'completion_tokens': usage.get(completion_key),
        'total_tokens': usage.get(total_key),
    }
    if cached_key:
        normalized['cache_read_input_tokens'] = usage.get(cached_key)
    return normalized


def normalize_iflow_usage(metadata: dict[str, Any]) -> dict[str, Any] | None:
    return normalize_token_usage(
        metadata.get('tokenUsage'),
        prompt_key='input',
        completion_key='output',
        total_key='total',
    )


def get_provider_session_id(debug_session: dict[str, Any], provider_type: str) -> str | None:
    provider_sessions = debug_session.get('providerSessions')
    if isinstance(provider_sessions, dict):
        provider_session_id = str(provider_sessions.get(provider_type) or '').strip()
        if provider_session_id:
            return provider_session_id

    if provider_type == 'codex_cli':
        legacy_thread_id = str(debug_session.get('codexThreadId') or '').strip()
        return legacy_thread_id or None
    return None


def parse_qwen_json_output(stdout: str) -> tuple[str | None, str | None, dict[str, Any] | None]:
    if not stdout:
        return None, None, None

    try:
        payload = json.loads(stdout)
    except json.JSONDecodeError:
        return None, None, None

    events = payload if isinstance(payload, list) else [payload] if isinstance(payload, dict) else []
    session_id: str | None = None
    message: str | None = None
    usage: dict[str, Any] | None = None

    for event in events:
        if not isinstance(event, dict):
            continue

        session_id = event.get('session_id') or event.get('sessionId') or session_id

        if event.get('type') == 'assistant':
            event_message = event.get('message') or {}
            candidate = extract_assistant_text(event_message.get('content'))
            if candidate:
                message = candidate
        elif event.get('type') == 'result':
            candidate = str(event.get('result') or '').strip()
            if candidate:
                message = candidate
            usage = normalize_token_usage(
                event.get('usage'),
                prompt_key='input_tokens',
                completion_key='output_tokens',
                total_key='total_tokens',
                cached_key='cache_read_input_tokens',
            ) or usage

    return session_id, message, usage


def build_provider_command(provider_status: dict[str, Any]) -> list[str]:
    executable = provider_status.get('path') or provider_status.get('command') or ''
    if str(executable).lower().endswith('.ps1'):
        return ['powershell', '-ExecutionPolicy', 'Bypass', '-File', executable]
    return [executable]


def run_codex_cli(
    *,
    prompt: str,
    image_path: Path | None,
    model: str,
    working_dir: Path,
    thread_id: str | None,
) -> dict[str, Any]:
    provider_status = get_local_provider_statuses()['codex_cli']
    if not provider_status['available']:
        raise RuntimeError('未检测到 Codex CLI，请先在本机安装并登录 Codex。')

    output_fd, output_name = tempfile.mkstemp(prefix='ev-log-codex-', suffix='.txt')
    os.close(output_fd)
    output_path = Path(output_name)
    base_command = build_provider_command(provider_status)

    if thread_id:
        command = [
            *base_command,
            'exec',
            'resume',
            thread_id,
            '--json',
            '--skip-git-repo-check',
            '-o',
            str(output_path),
        ]
    else:
        command = [
            *base_command,
            'exec',
            '--json',
            '--skip-git-repo-check',
            '--sandbox',
            'read-only',
            '--color',
            'never',
            '-o',
            str(output_path),
        ]

    if model:
        command.extend(['-m', model])
    if image_path and image_path.is_file():
        command.extend(['-i', str(image_path)])
    command.append('-')

    try:
        result = subprocess.run(
            command,
            input=prompt,
            capture_output=True,
            text=True,
            encoding='utf-8',
            errors='replace',
            timeout=240,
            cwd=str(working_dir),
        )
        stdout = result.stdout.strip()
        stderr = result.stderr.strip()
        thread_id_from_output, message_from_output, usage = parse_codex_json_output(stdout)
        last_message = output_path.read_text(encoding='utf-8', errors='replace').strip() if output_path.exists() else ''
    finally:
        output_path.unlink(missing_ok=True)

    if result.returncode != 0:
        raise RuntimeError(stderr or stdout or 'Codex CLI 调用失败。')

    message = last_message or message_from_output
    if not message:
        raise RuntimeError('Codex CLI 没有返回可解析的回答内容。')

    return {
        'message': message,
        'usage': usage,
        'model': model or 'codex_cli',
        'sessionId': thread_id_from_output or thread_id,
        'threadId': thread_id_from_output or thread_id,
    }


def run_qwen_cli(
    *,
    prompt: str,
    model: str,
    session_id: str | None,
) -> dict[str, Any]:
    provider_status = get_local_provider_statuses()['qwen_cli']
    if not provider_status['available']:
        raise RuntimeError('未检测到 Qwen Code，请先在本机安装并登录。')

    command = [*build_provider_command(provider_status), '--chat-recording', 'true', '--output-format', 'json']
    if session_id:
        command.extend(['-r', session_id])
    command.extend(['-p', prompt])
    if model:
        command.extend(['-m', model])

    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        encoding='utf-8',
        errors='replace',
        timeout=240,
    )

    stdout = result.stdout.strip()
    stderr = result.stderr.strip()
    if result.returncode != 0:
        raise RuntimeError(stderr or stdout or 'Qwen Code 调用失败。')
    parsed_session_id, message, usage = parse_qwen_json_output(stdout)
    if not message:
        raise RuntimeError('Qwen Code 没有返回可解析的回答内容。')

    return {
        'message': message,
        'usage': usage,
        'model': model or 'qwen_cli',
        'sessionId': parsed_session_id or session_id,
        'threadId': None,
    }


def run_iflow_cli(
    *,
    prompt: str,
    model: str,
    session_id: str | None,
) -> dict[str, Any]:
    provider_status = get_local_provider_statuses()['iflow_cli']
    if not provider_status['available']:
        raise RuntimeError('未检测到 iFlow CLI，请先在本机安装并登录。')

    output_fd, output_name = tempfile.mkstemp(prefix='ev-log-iflow-', suffix='.json')
    os.close(output_fd)
    output_path = Path(output_name)

    command = [*build_provider_command(provider_status)]
    if session_id:
        command.extend(['-r', session_id])
    command.extend(['-p', prompt, '-o', str(output_path)])
    if model:
        command.extend(['-m', model])

    try:
        result = subprocess.run(
            command,
            capture_output=True,
            text=True,
            encoding='utf-8',
            errors='replace',
            timeout=240,
        )
        stdout = result.stdout.strip()
        stderr = result.stderr.strip()
        output_payload = output_path.read_text(encoding='utf-8', errors='replace') if output_path.exists() else ''
        metadata = safe_json_loads(output_payload) or parse_iflow_execution_info(output_payload, stdout, stderr)
    finally:
        output_path.unlink(missing_ok=True)

    if result.returncode != 0:
        raise RuntimeError(stderr or stdout or 'iFlow CLI 调用失败。')

    message = extract_iflow_message(stdout)
    if not message:
        raise RuntimeError('iFlow CLI 没有返回可解析的回答内容。')

    return {
        'message': message,
        'usage': normalize_iflow_usage(metadata),
        'model': model or 'iflow_cli',
        'sessionId': str(metadata.get('session-id') or session_id or '').strip() or None,
        'threadId': None,
    }


def call_openai_compatible_sync(
    *,
    request_data: ChatRequest,
    debug_session: dict[str, Any],
    run: dict[str, Any],
    image_path: Path | None,
) -> dict[str, Any]:
    settings = request_data.settings
    base_url = normalize_base_url(settings.baseUrl)
    api_key = settings.apiKey.strip()
    model = settings.model.strip()

    if not request_data.message.strip():
        raise ValueError('请输入要发送给助手的内容。')
    if not base_url or not api_key or not model:
        raise ValueError('远程 API 模式需要完整填写 Base URL、API Key 和 Model。')

    image_data_url = build_image_data_url(image_path) if settings.includeImage else None
    upstream_messages = build_remote_messages(
        debug_session=debug_session,
        run=run,
        user_message=request_data.message,
        image_data_url=image_data_url,
    )

    payload = json.dumps(
        {
            'model': model,
            'temperature': 0.3,
            'messages': upstream_messages,
        },
        ensure_ascii=False,
    ).encode('utf-8')

    upstream_request = urllib_request.Request(
        f'{base_url}/chat/completions',
        data=payload,
        method='POST',
        headers={
            'Content-Type': 'application/json',
            'Authorization': f'Bearer {api_key}',
        },
    )

    try:
        with urllib_request.urlopen(upstream_request, timeout=90) as response:
            response_text = response.read().decode('utf-8', errors='replace')
    except urllib_error.HTTPError as error:
        response_text = error.read().decode('utf-8', errors='replace')
        payload_obj = safe_json_loads(response_text)
        error_message = payload_obj.get('error', {}).get('message') if isinstance(payload_obj.get('error'), dict) else None
        raise RuntimeError(error_message or response_text or f'上游接口返回 {error.code}') from error
    except urllib_error.URLError as error:
        raise RuntimeError(f'无法连接上游接口: {error.reason}') from error

    payload_obj = safe_json_loads(response_text)
    assistant_text = extract_assistant_text(payload_obj.get('choices', [{}])[0].get('message', {}).get('content'))
    if not assistant_text:
        raise RuntimeError('上游接口没有返回可解析的回答内容。')

    return {
        'message': assistant_text,
        'usage': payload_obj.get('usage'),
        'model': payload_obj.get('model') or model,
        'threadId': None,
    }


def call_chat_completion_sync(
    *,
    request_data: ChatRequest,
    debug_session: dict[str, Any],
    run: dict[str, Any],
    image_path: Path | None,
    working_dir: Path,
) -> dict[str, Any]:
    provider_type = request_data.settings.providerType
    if not request_data.message.strip():
        raise ValueError('请输入要发送给助手的内容。')

    if provider_type == 'openai_compatible':
        return call_openai_compatible_sync(
            request_data=request_data,
            debug_session=debug_session,
            run=run,
            image_path=image_path,
        )

    if provider_type == 'qwen_cli':
        provider_session_id = get_provider_session_id(debug_session, provider_type)
        prompt = build_qwen_prompt(
            debug_session=debug_session,
            run=run,
            user_message=request_data.message,
            include_history=not bool(provider_session_id),
        )
        return run_qwen_cli(
            prompt=prompt,
            model=request_data.settings.model.strip(),
            session_id=provider_session_id,
        )

    if provider_type == 'iflow_cli':
        provider_session_id = get_provider_session_id(debug_session, provider_type)
        prompt = build_qwen_prompt(
            debug_session=debug_session,
            run=run,
            user_message=request_data.message,
            include_history=not bool(provider_session_id),
        )
        return run_iflow_cli(
            prompt=prompt,
            model=request_data.settings.model.strip(),
            session_id=provider_session_id,
        )

    if provider_type != 'codex_cli':
        raise ValueError('当前仅支持 Codex CLI、Qwen Code、iFlow CLI 或 OpenAI-compatible API。')

    prompt = build_codex_prompt(
        debug_session=debug_session,
        run=run,
        user_message=request_data.message,
    )
    return run_codex_cli(
        prompt=prompt,
        image_path=image_path if request_data.settings.includeImage else None,
        model=request_data.settings.model.strip(),
        working_dir=working_dir,
        thread_id=get_provider_session_id(debug_session, provider_type),
    )


async def call_chat_completion(
    *,
    request_data: ChatRequest,
    debug_session: dict[str, Any],
    run: dict[str, Any],
    image_path: Path | None,
    working_dir: Path,
) -> dict[str, Any]:
    return await asyncio.to_thread(
        call_chat_completion_sync,
        request_data=request_data,
        debug_session=debug_session,
        run=run,
        image_path=image_path,
        working_dir=working_dir,
    )
