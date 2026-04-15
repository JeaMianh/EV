from __future__ import annotations

import copy
import json
import threading
import uuid
from pathlib import Path
from typing import Any, Callable

from .log_store import iso_utc_now

MAX_MESSAGE_HISTORY = 60
LOCAL_PROVIDER_KEYS = {"codex_cli", "qwen_cli", "iflow_cli"}


def make_session_id() -> str:
    return f"dbg-{uuid.uuid4().hex[:8]}"


def make_entry_id() -> str:
    return f"entry-{uuid.uuid4().hex[:10]}"


def normalize_text(value: Any) -> str:
    return str(value or "").strip()


class DebugSessionStore:
    def __init__(self, storage_path: Path):
        self.storage_path = storage_path
        self.data_lock = threading.RLock()
        self.sessions: list[dict[str, Any]] = []
        self.session_map: dict[str, dict[str, Any]] = {}
        self._load()

    def _default_session(self, *, title: str, goal: str) -> dict[str, Any]:
        now = iso_utc_now()
        return {
            "id": make_session_id(),
            "title": title or "Debug Session",
            "goal": goal,
            "codexThreadId": None,
            "providerSessions": {},
            "createdAt": now,
            "updatedAt": now,
            "entries": [],
            "messages": [],
        }

    def _normalize_session(self, payload: dict[str, Any]) -> dict[str, Any]:
        session = self._default_session(
            title=normalize_text(payload.get("title")) or "Debug Session",
            goal=normalize_text(payload.get("goal")),
        )
        session["id"] = normalize_text(payload.get("id")) or make_session_id()
        provider_sessions: dict[str, str] = {}
        raw_provider_sessions = payload.get("providerSessions")
        if isinstance(raw_provider_sessions, dict):
            for provider_type, provider_session_id in raw_provider_sessions.items():
                normalized_provider_type = normalize_text(provider_type)
                normalized_session_id = normalize_text(provider_session_id)
                if normalized_provider_type in LOCAL_PROVIDER_KEYS and normalized_session_id:
                    provider_sessions[normalized_provider_type] = normalized_session_id

        legacy_codex_thread_id = normalize_text(payload.get("codexThreadId")) or None
        if legacy_codex_thread_id and "codex_cli" not in provider_sessions:
            provider_sessions["codex_cli"] = legacy_codex_thread_id

        session["providerSessions"] = provider_sessions
        session["codexThreadId"] = provider_sessions.get("codex_cli")
        session["createdAt"] = normalize_text(payload.get("createdAt")) or session["createdAt"]
        session["updatedAt"] = normalize_text(payload.get("updatedAt")) or session["updatedAt"]

        entries: list[dict[str, Any]] = []
        for raw_entry in payload.get("entries") or []:
            run_id = normalize_text(raw_entry.get("runId"))
            if not run_id:
                continue
            entries.append(
                {
                    "id": normalize_text(raw_entry.get("id")) or make_entry_id(),
                    "runId": run_id,
                    "addedAt": normalize_text(raw_entry.get("addedAt")) or session["updatedAt"],
                    "changeNote": normalize_text(raw_entry.get("changeNote")),
                    "hypothesis": normalize_text(raw_entry.get("hypothesis")),
                    "resultNote": normalize_text(raw_entry.get("resultNote")),
                }
            )
        session["entries"] = entries

        messages: list[dict[str, Any]] = []
        for raw_message in payload.get("messages") or []:
            role = normalize_text(raw_message.get("role")) or "user"
            if role not in {"system", "assistant", "user"}:
                role = "user"
            content = normalize_text(raw_message.get("content"))
            if not content:
                continue
            messages.append(
                {
                    "role": role,
                    "content": content,
                    "createdAt": normalize_text(raw_message.get("createdAt")) or session["updatedAt"],
                    "runId": normalize_text(raw_message.get("runId")) or None,
                }
            )
        session["messages"] = messages[-MAX_MESSAGE_HISTORY:]
        return session

    def _rebuild_index_unlocked(self) -> None:
        self.sessions.sort(key=lambda item: (item.get("updatedAt") or "", item.get("id") or ""), reverse=True)
        self.session_map = {session["id"]: session for session in self.sessions}

    def _load(self) -> None:
        with self.data_lock:
            if not self.storage_path.exists():
                self.sessions = []
                self.session_map = {}
                return

            try:
                raw_payload = json.loads(self.storage_path.read_text(encoding="utf-8"))
            except (OSError, json.JSONDecodeError):
                self.sessions = []
                self.session_map = {}
                return

            raw_sessions = raw_payload.get("sessions") if isinstance(raw_payload, dict) else []
            self.sessions = [
                self._normalize_session(session_payload)
                for session_payload in raw_sessions
                if isinstance(session_payload, dict)
            ]
            self._rebuild_index_unlocked()

    def _save_unlocked(self) -> None:
        self.storage_path.parent.mkdir(parents=True, exist_ok=True)
        payload = {"sessions": self.sessions}
        self.storage_path.write_text(json.dumps(payload, ensure_ascii=False, indent=2), encoding="utf-8")

    def _touch_session_unlocked(self, session: dict[str, Any]) -> None:
        session["updatedAt"] = iso_utc_now()

    def _build_summary(self, session: dict[str, Any]) -> dict[str, Any]:
        entries = session.get("entries") or []
        messages = session.get("messages") or []
        last_entry = entries[-1] if entries else None
        return {
            "id": session["id"],
            "title": session["title"],
            "goal": session["goal"],
            "createdAt": session["createdAt"],
            "updatedAt": session["updatedAt"],
            "runCount": len(entries),
            "messageCount": len(messages),
            "lastRunId": last_entry.get("runId") if last_entry else None,
            "hasCodexThread": bool((session.get("providerSessions") or {}).get("codex_cli")),
            "providerSessions": copy.deepcopy(session.get("providerSessions") or {}),
        }

    def list_sessions(self) -> list[dict[str, Any]]:
        with self.data_lock:
            return [copy.deepcopy(self._build_summary(session)) for session in self.sessions]

    def get_session_count(self) -> int:
        with self.data_lock:
            return len(self.sessions)

    def create_session(self, *, title: str, goal: str) -> dict[str, Any]:
        with self.data_lock:
            session = self._default_session(title=normalize_text(title), goal=normalize_text(goal))
            self.sessions.append(session)
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return copy.deepcopy(session)

    def delete_session(self, session_id: str) -> bool:
        with self.data_lock:
            original_size = len(self.sessions)
            self.sessions = [session for session in self.sessions if session["id"] != session_id]
            if len(self.sessions) == original_size:
                return False
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return True

    def update_session(self, session_id: str, *, title: str | None = None, goal: str | None = None) -> dict[str, Any] | None:
        with self.data_lock:
            session = self.session_map.get(session_id)
            if not session:
                return None

            if title is not None:
                session["title"] = normalize_text(title) or session["title"]
            if goal is not None:
                session["goal"] = normalize_text(goal)
            self._touch_session_unlocked(session)
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return copy.deepcopy(session)

    def add_run_entry(
        self,
        session_id: str,
        *,
        run_id: str,
        change_note: str = "",
        hypothesis: str = "",
        result_note: str = "",
    ) -> tuple[dict[str, Any] | None, bool, str | None]:
        with self.data_lock:
            session = self.session_map.get(session_id)
            if not session:
                return None, False, None

            normalized_run_id = normalize_text(run_id)
            existing_entry = next((entry for entry in session["entries"] if entry.get("runId") == normalized_run_id), None)
            if existing_entry:
                if change_note:
                    existing_entry["changeNote"] = normalize_text(change_note)
                if hypothesis:
                    existing_entry["hypothesis"] = normalize_text(hypothesis)
                if result_note:
                    existing_entry["resultNote"] = normalize_text(result_note)
                self._touch_session_unlocked(session)
                self._rebuild_index_unlocked()
                self._save_unlocked()
                return copy.deepcopy(session), False, existing_entry["id"]

            entry_id = make_entry_id()
            session["entries"].append(
                {
                    "id": entry_id,
                    "runId": normalized_run_id,
                    "addedAt": iso_utc_now(),
                    "changeNote": normalize_text(change_note),
                    "hypothesis": normalize_text(hypothesis),
                    "resultNote": normalize_text(result_note),
                }
            )
            self._touch_session_unlocked(session)
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return copy.deepcopy(session), True, entry_id

    def remove_run_entry(self, session_id: str, entry_id: str) -> dict[str, Any] | None:
        with self.data_lock:
            session = self.session_map.get(session_id)
            if not session:
                return None

            original_size = len(session.get("entries") or [])
            session["entries"] = [entry for entry in session.get("entries") or [] if entry.get("id") != entry_id]
            if len(session["entries"]) == original_size:
                return None

            self._touch_session_unlocked(session)
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return copy.deepcopy(session)

    def record_chat_exchange(
        self,
        session_id: str,
        *,
        run_id: str,
        user_message: str,
        assistant_message: str,
    ) -> dict[str, Any] | None:
        with self.data_lock:
            session = self.session_map.get(session_id)
            if not session:
                return None

            now = iso_utc_now()
            if normalize_text(user_message):
                session["messages"].append(
                    {
                        "role": "user",
                        "content": normalize_text(user_message),
                        "createdAt": now,
                        "runId": normalize_text(run_id) or None,
                    }
                )
            if normalize_text(assistant_message):
                session["messages"].append(
                    {
                        "role": "assistant",
                        "content": normalize_text(assistant_message),
                        "createdAt": now,
                        "runId": normalize_text(run_id) or None,
                    }
                )

            session["messages"] = session["messages"][-MAX_MESSAGE_HISTORY:]
            self._touch_session_unlocked(session)
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return copy.deepcopy(session)

    def set_provider_session_id(
        self,
        session_id: str,
        provider_type: str,
        provider_session_id: str | None,
    ) -> dict[str, Any] | None:
        normalized_provider_type = normalize_text(provider_type)
        if normalized_provider_type not in LOCAL_PROVIDER_KEYS:
            return None

        normalized_session_id = normalize_text(provider_session_id) or None
        with self.data_lock:
            session = self.session_map.get(session_id)
            if not session:
                return None

            provider_sessions = session.setdefault("providerSessions", {})
            if normalized_session_id:
                provider_sessions[normalized_provider_type] = normalized_session_id
            else:
                provider_sessions.pop(normalized_provider_type, None)

            session["codexThreadId"] = provider_sessions.get("codex_cli")
            self._touch_session_unlocked(session)
            self._rebuild_index_unlocked()
            self._save_unlocked()
            return copy.deepcopy(session)

    def set_codex_thread_id(self, session_id: str, thread_id: str | None) -> dict[str, Any] | None:
        return self.set_provider_session_id(session_id, "codex_cli", thread_id)

    def get_session_snapshot(self, session_id: str) -> dict[str, Any] | None:
        with self.data_lock:
            session = self.session_map.get(session_id)
            return copy.deepcopy(session) if session else None

    def get_session_detail(
        self,
        session_id: str,
        resolve_run: Callable[[str], dict[str, Any] | None],
    ) -> dict[str, Any] | None:
        with self.data_lock:
            session = self.session_map.get(session_id)
            if not session:
                return None
            base = copy.deepcopy(session)

        enriched_entries: list[dict[str, Any]] = []
        for entry in base.get("entries") or []:
            run = resolve_run(entry["runId"])
            enriched_entries.append(
                {
                    **entry,
                    "runExists": bool(run),
                    "run": {
                        "id": run["id"],
                        "updatedAt": run["updatedAt"],
                        "createdAt": run["createdAt"],
                        "summary": copy.deepcopy(run["summary"]),
                        "simInfo": copy.deepcopy(run.get("simInfo")),
                    }
                    if run
                    else None,
                }
            )

        base["entries"] = enriched_entries
        base["summary"] = self._build_summary(base)
        return base

    def remove_run_references(self, run_id: str) -> int:
        removed_count = 0
        with self.data_lock:
            changed = False
            for session in self.sessions:
                original_size = len(session.get("entries") or [])
                session["entries"] = [entry for entry in session.get("entries") or [] if entry.get("runId") != run_id]
                if len(session["entries"]) != original_size:
                    removed_count += original_size - len(session["entries"])
                    self._touch_session_unlocked(session)
                    changed = True

            if changed:
                self._rebuild_index_unlocked()
                self._save_unlocked()

        return removed_count
