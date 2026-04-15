from __future__ import annotations

import asyncio
import logging
import time
from contextlib import asynccontextmanager, suppress
from pathlib import Path

from fastapi import FastAPI, HTTPException
from fastapi.responses import FileResponse, JSONResponse, StreamingResponse
from pydantic import BaseModel

from .chat_proxy import ChatRequest, call_chat_completion, get_local_provider_statuses
from .debug_sessions import DebugSessionStore
from .log_store import POLL_INTERVAL_SECONDS, SSE_HEARTBEAT_SECONDS, LogRunStore, format_sse

PROJECT_ROOT = Path(__file__).resolve().parent.parent
PUBLIC_ROOT = PROJECT_ROOT / 'public'
LOGS_ROOT = PROJECT_ROOT.parent
DATA_ROOT = PROJECT_ROOT / 'data'

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(levelname)s %(name)s %(message)s')
LOGGER = logging.getLogger(__name__)

store = LogRunStore(
    project_root=PROJECT_ROOT,
    logs_root=LOGS_ROOT,
    project_name=PROJECT_ROOT.name,
)
debug_session_store = DebugSessionStore(DATA_ROOT / 'debug_sessions.json')


class DebugSessionCreateRequest(BaseModel):
    title: str = ''
    goal: str = ''


class DebugSessionUpdateRequest(BaseModel):
    title: str | None = None
    goal: str | None = None


class DebugSessionRunRequest(BaseModel):
    runId: str
    changeNote: str = ''
    hypothesis: str = ''
    resultNote: str = ''


async def poll_logs_forever() -> None:
    while True:
        try:
            await store.refresh_runs('fs-poll')
        except Exception as error:  # noqa: BLE001
            LOGGER.exception('log refresh failed: %s', error)
        await asyncio.sleep(POLL_INTERVAL_SECONDS)


@asynccontextmanager
async def lifespan(_: FastAPI):
    await store.refresh_runs('startup', force=True)
    watcher_task = asyncio.create_task(poll_logs_forever())
    try:
        yield
    finally:
        watcher_task.cancel()
        with suppress(asyncio.CancelledError):
            await watcher_task


app = FastAPI(title='EV Log Copilot', lifespan=lifespan)


@app.get('/api/health')
def health() -> dict[str, object]:
    payload = store.get_health_payload()
    payload['localProviders'] = get_local_provider_statuses()
    payload['debugSessionCount'] = debug_session_store.get_session_count()
    return payload


@app.get('/api/runs')
def list_runs() -> dict[str, object]:
    return store.get_runs_payload()


@app.get('/api/runs/{run_id}', response_model=None)
def get_run(run_id: str):
    payload = store.get_run_detail(run_id)
    if not payload:
        return JSONResponse(status_code=404, content={'error': '未找到对应仿真目录。'})
    return payload


@app.delete('/api/runs/{run_id}', response_model=None)
async def delete_run(run_id: str):
    try:
        await store.delete_run(run_id)
    except FileNotFoundError:
        return JSONResponse(status_code=404, content={'error': '目录不存在或已经被删除。'})
    except PermissionError as error:
        return JSONResponse(status_code=403, content={'error': str(error)})

    debug_session_store.remove_run_references(run_id)
    return {'ok': True, 'deletedRunId': run_id}


@app.get('/api/runs/{run_id}/assets/{file_name:path}', response_model=None)
def get_asset(run_id: str, file_name: str):
    asset_path = store.resolve_asset_path(run_id, file_name)
    if not asset_path or not asset_path.is_file():
        return JSONResponse(status_code=404, content={'error': '文件不存在或不允许访问。'})
    return FileResponse(asset_path)


@app.get('/api/debug-sessions')
def list_debug_sessions() -> dict[str, object]:
    return {'sessions': debug_session_store.list_sessions()}


@app.post('/api/debug-sessions', response_model=None)
def create_debug_session(request_data: DebugSessionCreateRequest):
    session = debug_session_store.create_session(title=request_data.title, goal=request_data.goal)
    session_detail = debug_session_store.get_session_detail(session['id'], store.get_run_snapshot)
    return {'session': session_detail}


@app.get('/api/debug-sessions/{session_id}', response_model=None)
def get_debug_session(session_id: str):
    session = debug_session_store.get_session_detail(session_id, store.get_run_snapshot)
    if not session:
        return JSONResponse(status_code=404, content={'error': '未找到对应的调试会话。'})
    return {'session': session}


@app.patch('/api/debug-sessions/{session_id}', response_model=None)
def update_debug_session(session_id: str, request_data: DebugSessionUpdateRequest):
    session = debug_session_store.update_session(
        session_id,
        title=request_data.title,
        goal=request_data.goal,
    )
    if not session:
        return JSONResponse(status_code=404, content={'error': '未找到对应的调试会话。'})
    session_detail = debug_session_store.get_session_detail(session_id, store.get_run_snapshot)
    return {'session': session_detail}


@app.delete('/api/debug-sessions/{session_id}', response_model=None)
def delete_debug_session(session_id: str):
    deleted = debug_session_store.delete_session(session_id)
    if not deleted:
        return JSONResponse(status_code=404, content={'error': '未找到对应的调试会话。'})
    return {'ok': True, 'deletedSessionId': session_id}


@app.post('/api/debug-sessions/{session_id}/runs', response_model=None)
def add_run_to_debug_session(session_id: str, request_data: DebugSessionRunRequest):
    if not store.get_run_snapshot(request_data.runId):
        return JSONResponse(status_code=404, content={'error': '要加入的仿真目录不存在。'})

    session, added, entry_id = debug_session_store.add_run_entry(
        session_id,
        run_id=request_data.runId,
        change_note=request_data.changeNote,
        hypothesis=request_data.hypothesis,
        result_note=request_data.resultNote,
    )
    if not session:
        return JSONResponse(status_code=404, content={'error': '未找到对应的调试会话。'})
    session_detail = debug_session_store.get_session_detail(session_id, store.get_run_snapshot)
    return {'session': session_detail, 'added': added, 'entryId': entry_id}


@app.delete('/api/debug-sessions/{session_id}/runs/{entry_id}', response_model=None)
def remove_run_from_debug_session(session_id: str, entry_id: str):
    session = debug_session_store.remove_run_entry(session_id, entry_id)
    if not session:
        return JSONResponse(status_code=404, content={'error': '未找到对应的调试条目。'})
    session_detail = debug_session_store.get_session_detail(session_id, store.get_run_snapshot)
    return {'session': session_detail, 'deletedEntryId': entry_id}


@app.get('/api/events')
async def events() -> StreamingResponse:
    queue = await store.subscribe()
    hello_payload = store.get_hello_payload()

    async def event_stream():
        try:
            yield format_sse('hello', hello_payload)
            while True:
                try:
                    event, payload = await asyncio.wait_for(queue.get(), timeout=SSE_HEARTBEAT_SECONDS)
                    yield format_sse(event, payload)
                except asyncio.TimeoutError:
                    yield format_sse('heartbeat', {'ts': int(time.time() * 1000)})
        finally:
            await store.unsubscribe(queue)

    return StreamingResponse(
        event_stream(),
        media_type='text/event-stream',
        headers={
            'Cache-Control': 'no-cache',
            'Connection': 'keep-alive',
            'X-Accel-Buffering': 'no',
        },
    )


@app.post('/api/chat', response_model=None)
async def chat(request_data: ChatRequest):
    run = store.get_run_snapshot(request_data.runId)
    if not run:
        return JSONResponse(status_code=400, content={'error': '请先选择一个仿真目录。'})

    session = debug_session_store.get_session_detail(request_data.debugSessionId, store.get_run_snapshot)
    if not session:
        return JSONResponse(status_code=400, content={'error': '请先创建或选择一个调试会话。'})

    image_name = request_data.imageName or run.get('previewImage')
    image_path = store.resolve_asset_path(run['id'], image_name) if image_name else None

    try:
        response_payload = await call_chat_completion(
            request_data=request_data,
            debug_session=session,
            run=run,
            image_path=image_path,
            working_dir=PROJECT_ROOT,
        )
    except ValueError as error:
        return JSONResponse(status_code=400, content={'error': str(error)})
    except RuntimeError as error:
        return JSONResponse(status_code=502, content={'error': str(error)})

    provider_session_id = response_payload.get('sessionId') or response_payload.get('threadId')
    if provider_session_id:
        debug_session_store.set_provider_session_id(
            request_data.debugSessionId,
            request_data.settings.providerType,
            provider_session_id,
        )

    debug_session_store.record_chat_exchange(
        request_data.debugSessionId,
        run_id=request_data.runId,
        user_message=request_data.message,
        assistant_message=response_payload['message'],
    )
    session_detail = debug_session_store.get_session_detail(request_data.debugSessionId, store.get_run_snapshot)

    return {
        'message': response_payload['message'],
        'usage': response_payload.get('usage'),
        'model': response_payload.get('model'),
        'sessionId': response_payload.get('sessionId'),
        'threadId': response_payload.get('threadId'),
        'session': session_detail,
    }


@app.get('/')
def index() -> FileResponse:
    return FileResponse(PUBLIC_ROOT / 'index.html')


@app.get('/settings')
@app.get('/settings/')
def settings_page() -> FileResponse:
    return FileResponse(PUBLIC_ROOT / 'settings.html')


@app.get('/sessions')
@app.get('/sessions/')
def sessions_page() -> FileResponse:
    return FileResponse(PUBLIC_ROOT / 'sessions.html')


@app.get('/{file_path:path}')
def static_or_spa(file_path: str) -> FileResponse:
    if file_path.startswith('api/'):
        raise HTTPException(status_code=404, detail='未找到接口。')

    candidate = (PUBLIC_ROOT / file_path).resolve()
    public_root = PUBLIC_ROOT.resolve()

    if str(candidate).startswith(str(public_root)) and candidate.is_file():
        return FileResponse(candidate)

    return FileResponse(PUBLIC_ROOT / 'index.html')
