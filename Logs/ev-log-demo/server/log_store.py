from __future__ import annotations

import asyncio
import copy
import hashlib
import json
import logging
import math
import shutil
import threading
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any
from urllib.parse import quote

LOGGER = logging.getLogger(__name__)

SERIES_KEYS = (
    "Time",
    "EgoSpd",
    "TgtSpd",
    "GT_Dist",
    "Radar_Dist",
    "Radar_RelVel",
    "Target_ID",
    "Flag_Ghost",
    "Flag_Loss",
)
PREFERRED_IMAGES = (
    "Analysis_Report.png",
    "Analysis_Plot.png",
    "Coordinate_Analysis.png",
    "Result_Plot.png",
)
SSE_HEARTBEAT_SECONDS = 15.0
POLL_INTERVAL_SECONDS = 1.5


def iso_utc_now() -> str:
    return datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")


def iso_utc_from_timestamp(timestamp: float) -> str:
    return datetime.fromtimestamp(timestamp, tz=timezone.utc).isoformat().replace("+00:00", "Z")


def to_optional_number(value: Any) -> float | None:
    if value is None or isinstance(value, bool):
        return None

    try:
        number = float(value)
    except (TypeError, ValueError):
        return None

    return number if math.isfinite(number) else None


def to_number_array(values: Any) -> list[float | None]:
    if isinstance(values, (list, tuple)):
        return [to_optional_number(value) for value in values]

    scalar = to_optional_number(values)
    return [scalar] if scalar is not None else []


def round_number(value: float | None, digits: int = 3) -> float | None:
    if value is None or not math.isfinite(value):
        return None

    return round(value, digits)


def format_percent(value: float | None) -> float | None:
    if value is None or not math.isfinite(value):
        return None

    return round_number(value * 100, 1)


def detect_radar_validity(distance: float | None) -> bool:
    return distance is not None and math.isfinite(distance) and distance < 199.5


def safe_json_loads(raw_text: str | None) -> dict[str, Any] | None:
    if not raw_text:
        return None

    try:
        payload = json.loads(raw_text)
    except json.JSONDecodeError:
        return None

    return payload if isinstance(payload, dict) else None


def average(values: list[float]) -> float | None:
    return sum(values) / len(values) if values else None


def max_abs(values: list[float]) -> float | None:
    return max((abs(value) for value in values), default=None)


def safe_at(values: list[float | None], index: int | None) -> float | None:
    if index is None or index < 0 or index >= len(values):
        return None

    value = values[index]
    return value if value is not None and math.isfinite(value) else None


def normalize_series(log_data: dict[str, Any] | None) -> tuple[int, dict[str, list[float | None]]]:
    source = log_data if isinstance(log_data, dict) else {}
    raw_time = to_number_array(source.get("Time"))
    series = {key: to_number_array(source.get(key)) for key in SERIES_KEYS}
    series["Time"] = raw_time

    length_candidates = [len(values) for values in series.values() if values]
    if not length_candidates:
        return 0, series

    fallback_length = max(length_candidates)
    time_values = raw_time if raw_time else [float(index) for index in range(fallback_length)]
    common_length = min([len(time_values), *length_candidates])

    normalized = {
        key: (time_values[:common_length] if key == "Time" else values[:common_length])
        for key, values in series.items()
    }
    return common_length, normalized


def build_insights(summary: dict[str, Any], sim_info: dict[str, Any] | None) -> list[str]:
    insights: list[str] = []

    if summary["sampleCount"] < 10:
        insights.append("当前日志采样点很少，更像调试切片。建议先确认仿真是否完整结束，再评估控制或感知质量。")

    detection_coverage = summary["detectionCoverage"]
    if detection_coverage is not None and detection_coverage < 0.2:
        insights.append("雷达有效检测覆盖率很低，目标大部分时间未被稳定观测。优先检查安装俯仰角、目标进入视场条件和检测门限。")
    elif detection_coverage is not None and detection_coverage < 0.7:
        insights.append("雷达已出现间歇性锁定，但仍有明显掉点。可以继续调试目标关联逻辑、距离门限和滤波参数。")

    loss_ratio = summary["lossRatio"]
    if loss_ratio is not None and loss_ratio > 0.5:
        insights.append("目标丢失比例偏高，感知链路仍不稳定。建议先排查目标 ID 建立条件以及 `Flag_Loss` 触发逻辑。")

    distance_error = summary["avgAbsDistanceError"]
    if distance_error is not None and distance_error > 10:
        insights.append("雷达距离与真值偏差较大，存在标定或时序对齐问题。建议核对坐标系、时间戳对齐以及量测噪声设置。")
    elif distance_error is not None and distance_error > 3:
        insights.append("距离误差已经可见，但还不至于完全失效。可以继续微调滤波、目标匹配与外参。")

    if summary["ghostCount"] > 0:
        insights.append("日志里出现 ghost 标记，说明误检仍在发生。建议在后处理阶段增加稳定性判定和多帧一致性约束。")

    final_closing_speed = summary["finalClosingSpeed"]
    if final_closing_speed is not None and final_closing_speed < -15:
        insights.append("末端相对速度绝对值较大，若仍无法稳定跟踪，优先检查高速闭合场景下的更新周期和关联窗口。")

    if isinstance(sim_info, dict):
        parts: list[str] = []
        rcs = to_optional_number(sim_info.get("RCS_Setting"))
        pitch = to_optional_number(sim_info.get("Pitch_Setting"))
        if rcs is not None:
            parts.append(f"RCS={sim_info.get('RCS_Setting')}")
        if pitch is not None:
            parts.append(f"Pitch={sim_info.get('Pitch_Setting')}")
        if parts:
            insights.append(f"当前仿真参数为 {'，'.join(parts)}。后续可以围绕这组参数做批量对比，找出对锁定率最敏感的配置。")

    if not insights:
        insights.append("这一轮仿真没有暴露特别明显的异常，可继续结合更多 run 做横向对比，确认表现是否稳定。")

    return insights


def build_summary(log_data: dict[str, Any] | None) -> tuple[dict[str, Any], dict[str, list[float | None]]]:
    sample_count, series = normalize_series(log_data)
    time_values = series["Time"]
    ego_speed = series["EgoSpd"]
    target_speed = series["TgtSpd"]
    ground_truth_distance = series["GT_Dist"]
    radar_distance = series["Radar_Dist"]
    radar_relative_velocity = series["Radar_RelVel"]
    target_id = series["Target_ID"]
    flag_ghost = series["Flag_Ghost"]
    flag_loss = series["Flag_Loss"]

    radar_valid_mask = [1 if detect_radar_validity(value) else 0 for value in radar_distance]
    valid_radar_count = sum(radar_valid_mask)
    loss_count_from_flag = sum(1 for value in flag_loss if (value or 0) > 0)
    loss_count = loss_count_from_flag if loss_count_from_flag > 0 else max(sample_count - valid_radar_count, 0)
    ghost_count = sum(1 for value in flag_ghost if (value or 0) > 0)
    lock_count = sum(1 for value in target_id if (value or 0) > 0) if target_id else valid_radar_count

    distance_errors: list[float] = []
    for index, distance in enumerate(radar_distance):
        ground_truth = safe_at(ground_truth_distance, index)
        if detect_radar_validity(distance) and ground_truth is not None:
            distance_errors.append(distance - ground_truth)

    final_index = sample_count - 1 if sample_count > 0 else None
    target_final_speed = safe_at(target_speed, final_index)
    ego_final_speed = safe_at(ego_speed, final_index)
    derived_closing_speed = None
    if target_final_speed is not None and ego_final_speed is not None:
        derived_closing_speed = target_final_speed - ego_final_speed

    summary = {
        "sampleCount": sample_count,
        "durationSeconds": round_number((safe_at(time_values, final_index) or 0) - (safe_at(time_values, 0) or 0), 3)
        if sample_count > 1
        else None,
        "detectionCoverage": round_number(valid_radar_count / sample_count, 3) if sample_count > 0 else None,
        "lossRatio": round_number(loss_count / sample_count, 3) if sample_count > 0 else None,
        "lockRatio": round_number(lock_count / sample_count, 3) if sample_count > 0 else None,
        "ghostCount": ghost_count,
        "validRadarCount": valid_radar_count,
        "avgAbsDistanceError": round_number(average([abs(value) for value in distance_errors]), 3),
        "maxAbsDistanceError": round_number(max_abs(distance_errors), 3),
        "egoFinalSpeed": round_number(ego_final_speed, 3),
        "targetFinalSpeed": round_number(target_final_speed, 3),
        "finalGroundTruthDistance": round_number(safe_at(ground_truth_distance, final_index), 3),
        "finalRadarDistance": round_number(safe_at(radar_distance, final_index), 3),
        "finalClosingSpeed": None,
    }
    summary["finalClosingSpeed"] = round_number(safe_at(radar_relative_velocity, final_index), 3)
    if summary["finalClosingSpeed"] is None:
        summary["finalClosingSpeed"] = round_number(derived_closing_speed, 3)
    summary["insights"] = build_insights(summary, log_data.get("SimInfo") if isinstance(log_data, dict) else None)

    enriched_series = dict(series)
    enriched_series["Radar_Valid"] = radar_valid_mask
    enriched_series["Radar_Dist_Visible"] = [value if detect_radar_validity(value) else None for value in radar_distance]
    return summary, enriched_series


def pick_preview_image(image_names: list[str]) -> str | None:
    if not image_names:
        return None

    for preferred in PREFERRED_IMAGES:
        if preferred in image_names:
            return preferred

    return image_names[0]


def build_asset_url(run_id: str, file_name: str) -> str:
    return f"/api/runs/{quote(run_id, safe='')}/assets/{quote(file_name, safe='')}"


def to_list_item(run: dict[str, Any]) -> dict[str, Any]:
    preview_image = run["previewImage"]
    return {
        "id": run["id"],
        "updatedAt": run["updatedAt"],
        "createdAt": run["createdAt"],
        "imageCount": len(run["images"]),
        "matCount": len(run["matFiles"]),
        "simInfo": run["simInfo"],
        "summary": run["summary"],
        "previewImage": {
            "name": preview_image,
            "url": build_asset_url(run["id"], preview_image),
        }
        if preview_image
        else None,
    }


def to_detail_item(run: dict[str, Any]) -> dict[str, Any]:
    detail = to_list_item(run)
    detail.update(
        {
            "jsonFile": run["jsonFile"],
            "rawKeys": run["rawKeys"],
            "images": [{"name": name, "url": build_asset_url(run["id"], name)} for name in run["images"]],
            "matFiles": run["matFiles"],
            "series": run["series"],
        }
    )
    return detail


def build_run_context(run: dict[str, Any]) -> str:
    summary = run["summary"]
    sim_info = run.get("simInfo") or {}
    settings_text = "\n".join(f"{key}: {value}" for key, value in sim_info.items()) if sim_info else "无"

    return "\n".join(
        [
            f"当前仿真目录: {run['id']}",
            f"原始字段: {', '.join(run.get('rawKeys') or []) or '无'}",
            f"采样点数量: {summary['sampleCount']}",
            f"仿真时长(s): {summary['durationSeconds'] if summary['durationSeconds'] is not None else '无'}",
            f"雷达检测覆盖率: {format_percent(summary['detectionCoverage']) if summary['detectionCoverage'] is not None else '无'}%",
            f"目标丢失比例: {format_percent(summary['lossRatio']) if summary['lossRatio'] is not None else '无'}%",
            f"目标锁定比例: {format_percent(summary['lockRatio']) if summary['lockRatio'] is not None else '无'}%",
            f"平均距离误差(m): {summary['avgAbsDistanceError'] if summary['avgAbsDistanceError'] is not None else '无'}",
            f"最大距离误差(m): {summary['maxAbsDistanceError'] if summary['maxAbsDistanceError'] is not None else '无'}",
            f"末端自车速度(m/s): {summary['egoFinalSpeed'] if summary['egoFinalSpeed'] is not None else '无'}",
            f"末端目标速度(m/s): {summary['targetFinalSpeed'] if summary['targetFinalSpeed'] is not None else '无'}",
            f"末端真值距离(m): {summary['finalGroundTruthDistance'] if summary['finalGroundTruthDistance'] is not None else '无'}",
            f"末端雷达距离(m): {summary['finalRadarDistance'] if summary['finalRadarDistance'] is not None else '无'}",
            f"末端相对速度(m/s): {summary['finalClosingSpeed'] if summary['finalClosingSpeed'] is not None else '无'}",
            f"启发式建议: {' '.join(summary.get('insights') or [])}",
            f"仿真参数:\n{settings_text}",
        ]
    )


def collect_run(run_dir: Path) -> dict[str, Any] | None:
    run_stat = run_dir.stat()
    file_entries = []
    for entry in run_dir.iterdir():
        if not entry.is_file():
            continue

        lower_name = entry.name.lower()
        if lower_name == "simlog.json" or lower_name.endswith(".png") or lower_name.endswith(".mat"):
            file_stat = entry.stat()
            file_entries.append((entry.name, file_stat.st_mtime_ns, file_stat.st_size))

    if not file_entries:
        return None

    file_names = [name for name, _, _ in file_entries]
    json_file = next((name for name in file_names if name.lower() == "simlog.json"), None)
    image_names = sorted(name for name in file_names if name.lower().endswith(".png"))
    mat_files = sorted(name for name in file_names if name.lower().endswith(".mat"))

    raw_log = None
    if json_file:
        try:
            raw_log = (run_dir / json_file).read_text(encoding="utf-8-sig", errors="replace")
        except OSError:
            raw_log = None

    log_data = safe_json_loads(raw_log)
    summary, series = build_summary(log_data)

    return {
        "id": run_dir.name,
        "createdAt": iso_utc_from_timestamp(run_stat.st_ctime),
        "updatedAt": iso_utc_from_timestamp(run_stat.st_mtime),
        "jsonFile": json_file,
        "images": image_names,
        "matFiles": mat_files,
        "previewImage": pick_preview_image(image_names),
        "simInfo": log_data.get("SimInfo") if isinstance(log_data, dict) else None,
        "rawKeys": list(log_data.keys()) if isinstance(log_data, dict) else [],
        "summary": summary,
        "series": series,
        "_manifestSignature": [f"{name}:{mtime_ns}:{size}" for name, mtime_ns, size in sorted(file_entries)],
    }


def scan_runs(logs_root: Path, project_name: str) -> tuple[str, list[dict[str, Any]]]:
    runs: list[dict[str, Any]] = []
    signature_parts: list[dict[str, Any]] = []

    if not logs_root.exists():
        return "", runs

    for entry in logs_root.iterdir():
        if not entry.is_dir():
            continue
        if entry.name in {project_name, "node_modules", "__pycache__", ".git"}:
            continue

        try:
            run = collect_run(entry)
        except OSError as error:
            LOGGER.warning("skip run %s: %s", entry.name, error)
            continue

        if not run:
            continue

        signature_parts.append({"id": run["id"], "manifest": run["_manifestSignature"]})
        runs.append(run)

    runs.sort(key=lambda item: (item["updatedAt"], item["id"]), reverse=True)
    signature = hashlib.sha1(
        json.dumps(signature_parts, ensure_ascii=False, sort_keys=True).encode("utf-8")
    ).hexdigest()

    for run in runs:
        run.pop("_manifestSignature", None)

    return signature, runs


def format_sse(event: str, payload: dict[str, Any]) -> bytes:
    data = json.dumps(payload, ensure_ascii=False)
    return f"event: {event}\ndata: {data}\n\n".encode("utf-8")


@dataclass
class LogRunStore:
    project_root: Path
    logs_root: Path
    project_name: str
    runs: list[dict[str, Any]] = field(default_factory=list)
    run_map: dict[str, dict[str, Any]] = field(default_factory=dict)
    version: str | None = None
    last_scanned_at: str | None = None
    signature: str | None = None
    subscribers: set[asyncio.Queue[tuple[str, dict[str, Any]]]] = field(default_factory=set)
    data_lock: threading.RLock = field(default_factory=threading.RLock)

    async def refresh_runs(self, reason: str = "manual", force: bool = False) -> bool:
        signature, runs = await asyncio.to_thread(scan_runs, self.logs_root, self.project_name)
        scan_time = iso_utc_now()

        with self.data_lock:
            if not force and signature == self.signature:
                self.last_scanned_at = scan_time
                return False

            self.signature = signature
            self.runs = runs
            self.run_map = {run["id"]: run for run in runs}
            self.version = scan_time
            self.last_scanned_at = scan_time
            payload = {
                "version": self.version,
                "runCount": len(self.runs),
                "reason": reason,
            }

        await self.broadcast("runs_updated", payload)
        return True

    async def broadcast(self, event: str, payload: dict[str, Any]) -> None:
        with self.data_lock:
            subscribers = list(self.subscribers)

        for queue in subscribers:
            try:
                queue.put_nowait((event, payload))
            except asyncio.QueueFull:
                continue

    async def subscribe(self) -> asyncio.Queue[tuple[str, dict[str, Any]]]:
        queue: asyncio.Queue[tuple[str, dict[str, Any]]] = asyncio.Queue()
        with self.data_lock:
            self.subscribers.add(queue)
        return queue

    async def unsubscribe(self, queue: asyncio.Queue[tuple[str, dict[str, Any]]]) -> None:
        with self.data_lock:
            self.subscribers.discard(queue)

    def get_health_payload(self) -> dict[str, Any]:
        with self.data_lock:
            return {
                "ok": True,
                "logsRoot": str(self.logs_root),
                "projectRoot": str(self.project_root),
                "version": self.version,
                "runCount": len(self.runs),
            }

    def get_hello_payload(self) -> dict[str, Any]:
        with self.data_lock:
            return {
                "version": self.version,
                "runCount": len(self.runs),
            }

    def get_runs_payload(self) -> dict[str, Any]:
        with self.data_lock:
            return {
                "version": self.version,
                "lastScannedAt": self.last_scanned_at,
                "runs": [to_list_item(run) for run in self.runs],
            }

    def get_run_detail(self, run_id: str) -> dict[str, Any] | None:
        with self.data_lock:
            run = self.run_map.get(run_id)
            if not run:
                return None

            return {
                "version": self.version,
                "run": copy.deepcopy(to_detail_item(run)),
            }

    def get_run_snapshot(self, run_id: str) -> dict[str, Any] | None:
        with self.data_lock:
            run = self.run_map.get(run_id)
            return copy.deepcopy(run) if run else None

    def resolve_asset_path(self, run_id: str, file_name: str) -> Path | None:
        with self.data_lock:
            run = self.run_map.get(run_id)
            if not run:
                return None

            safe_name = Path(file_name).name
            allowed_files = {name for name in [run["jsonFile"], *run["images"], *run["matFiles"]] if name}
            if safe_name not in allowed_files:
                return None

            return self.logs_root / run_id / safe_name

    async def delete_run(self, run_id: str) -> None:
        await asyncio.to_thread(self._delete_run_sync, run_id)
        await self.refresh_runs("run-deleted", force=True)

    def _delete_run_sync(self, run_id: str) -> None:
        target_path = (self.logs_root / run_id).resolve()
        logs_root = self.logs_root.resolve()

        if target_path.parent != logs_root:
            raise FileNotFoundError("目录不存在。")
        if not target_path.exists() or not target_path.is_dir():
            raise FileNotFoundError("目录不存在。")
        if target_path.name in {self.project_name, "node_modules", "__pycache__", ".git"}:
            raise PermissionError("不允许删除该目录。")

        shutil.rmtree(target_path)
