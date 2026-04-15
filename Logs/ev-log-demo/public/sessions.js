const SettingsStore = window.EVLogSettings;

const DEBUG_SESSION_STORAGE_KEY = 'ev-log-demo-selected-debug-session';
const RUN_SELECTION_STORAGE_KEY = 'ev-log-demo-selected-run-id';

function loadSelectedDebugSessionId() {
  return localStorage.getItem(DEBUG_SESSION_STORAGE_KEY) || null;
}

function saveSelectedDebugSessionId(sessionId) {
  if (sessionId) {
    localStorage.setItem(DEBUG_SESSION_STORAGE_KEY, sessionId);
    return;
  }
  localStorage.removeItem(DEBUG_SESSION_STORAGE_KEY);
}

function getRequestedRunId() {
  const queryRunId = new URLSearchParams(window.location.search).get('runId');
  return queryRunId || localStorage.getItem(RUN_SELECTION_STORAGE_KEY) || null;
}

const state = {
  settings: SettingsStore.loadSettings(),
  health: null,
  requestedRunId: getRequestedRunId(),
  runs: [],
  sessions: [],
  selectedSessionId: loadSelectedDebugSessionId(),
  selectedSession: null,
  sidebarView: 'sessions',
  focusRunId: null,
  focusRunQuery: '',
  runDetails: {},
};

function getActiveProviderBusyText() {
  if (state.settings.providerType === 'iflow_cli') {
    return 'iFlow 正在分析当前调试会话…';
  }
  if (state.settings.providerType === 'qwen_cli') {
    return 'Qwen Code 正在分析当前调试会话…';
  }
  if (state.settings.providerType === 'codex_cli') {
    return 'Codex 正在分析当前调试会话…';
  }
  return '远程模型正在分析…';
}

function getSessionThreadStatusText() {
  const hasProviderSession = Boolean(state.selectedSession?.providerSessions?.[state.settings.providerType]);

  if (state.settings.providerType === 'codex_cli') {
    return hasProviderSession ? 'Codex 会话已绑定' : 'Codex 会话待建立';
  }
  if (state.settings.providerType === 'qwen_cli') {
    return hasProviderSession ? 'Qwen Code 会话已绑定' : 'Qwen Code 会话待建立';
  }
  if (state.settings.providerType === 'iflow_cli') {
    return hasProviderSession ? 'iFlow 会话已绑定' : 'iFlow 会话待建立';
  }
  return '远程 API 使用消息历史续聊';
}

function getSendButtonText() {
  const providerLabel = SettingsStore.getProviderLabel(state.settings);
  return `发送给 ${providerLabel}`;
}

const elements = {
  connectionDot: document.querySelector('#connectionDot'),
  connectionText: document.querySelector('#connectionText'),
  providerStatusText: document.querySelector('#providerStatusText'),
  sidebarKickerText: document.querySelector('#sidebarKickerText'),
  sidebarTitleText: document.querySelector('#sidebarTitleText'),
  sessionCountText: document.querySelector('#sessionCountText'),
  sessionList: document.querySelector('#sessionList'),
  sidebarBackButton: document.querySelector('#sidebarBackButton'),
  createSessionButton: document.querySelector('#createSessionButton'),
  sessionQuickSelect: document.querySelector('#sessionQuickSelect'),
  deleteCurrentSessionButton: document.querySelector('#deleteCurrentSessionButton'),
  sessionTitleHeading: document.querySelector('#sessionTitleHeading'),
  sessionMetaText: document.querySelector('#sessionMetaText'),
  sessionTitleInput: document.querySelector('#sessionTitleInput'),
  sessionGoalInput: document.querySelector('#sessionGoalInput'),
  saveSessionMetaButton: document.querySelector('#saveSessionMetaButton'),
  sessionThreadStatus: document.querySelector('#sessionThreadStatus'),
  sessionRunCountText: document.querySelector('#sessionRunCountText'),
  sessionMessageCountText: document.querySelector('#sessionMessageCountText'),
  sessionTimeline: document.querySelector('#sessionTimeline'),
  focusRunSearchInput: document.querySelector('#focusRunSearchInput'),
  focusRunSelect: document.querySelector('#focusRunSelect'),
  focusRunMetaText: document.querySelector('#focusRunMetaText'),
  focusRunImage: document.querySelector('#focusRunImage'),
  focusRunImageEmpty: document.querySelector('#focusRunImageEmpty'),
  focusRunCoverageText: document.querySelector('#focusRunCoverageText'),
  focusRunErrorText: document.querySelector('#focusRunErrorText'),
  focusRunUpdatedText: document.querySelector('#focusRunUpdatedText'),
  focusRunSummaryText: document.querySelector('#focusRunSummaryText'),
  addCurrentRunButton: document.querySelector('#addCurrentRunButton'),
  changeNoteInput: document.querySelector('#changeNoteInput'),
  hypothesisInput: document.querySelector('#hypothesisInput'),
  resultNoteInput: document.querySelector('#resultNoteInput'),
  openRunViewerButton: document.querySelector('#openRunViewerButton'),
  assistantSessionText: document.querySelector('#assistantSessionText'),
  chatStream: document.querySelector('#chatStream'),
  chatForm: document.querySelector('#chatForm'),
  chatInput: document.querySelector('#chatInput'),
  sendButton: document.querySelector('#sendButton'),
};

function formatDateTime(value) {
  if (!value) {
    return '未知时间';
  }
  const date = new Date(value);
  return Number.isNaN(date.getTime())
    ? value
    : new Intl.DateTimeFormat('zh-CN', { dateStyle: 'medium', timeStyle: 'short' }).format(date);
}

function formatPercent(value) {
  return Number.isFinite(value) ? `${(value * 100).toFixed(1)}%` : '-';
}

function formatMeters(value) {
  return Number.isFinite(value) ? `${value.toFixed(2)} m` : '-';
}

function setConnectionState(connected, text) {
  elements.connectionDot.classList.toggle('connected', connected);
  elements.connectionText.textContent = text;
}

function createTrashIconMarkup() {
  return `
    <svg viewBox="0 0 24 24" aria-hidden="true">
      <path d="M9 3h6l1 2h4v2H4V5h4l1-2Zm1 7h2v8h-2v-8Zm4 0h2v8h-2v-8ZM7 10h2v8H7v-8Zm-1 10h12l1-13H5l1 13Z"></path>
    </svg>
  `;
}

function renderProviderStatus() {
  if (state.settings.providerType === 'openai_compatible') {
    elements.providerStatusText.textContent = SettingsStore.isConfigured(state.settings)
      ? '远程 API 已配置'
      : '远程 API 未配置';
    return;
  }

  const providerKey = state.settings.providerType;
  const providerStatus = state.health?.localProviders?.[providerKey];
  const providerLabel = SettingsStore.getProviderLabel(state.settings);
  elements.providerStatusText.textContent = providerStatus?.available
    ? `${providerLabel} 已连接`
    : `${providerLabel} 未检测到`;
}

function dedupeRunIds(ids) {
  return [...new Set(ids.filter(Boolean))];
}

function getSessionRunIds() {
  return dedupeRunIds((state.selectedSession?.entries || []).map((entry) => entry.runId));
}

function getAllRunIds() {
  return dedupeRunIds((state.runs || []).map((run) => run.id));
}

function sessionContainsRun(runId) {
  return Boolean(runId && state.selectedSession?.entries?.some((entry) => entry.runId === runId));
}

function buildFocusRunOptionIds() {
  const pinnedIds = dedupeRunIds([state.focusRunId, state.requestedRunId, ...getSessionRunIds()]);
  const mergedIds = dedupeRunIds([...pinnedIds, ...getAllRunIds()]);
  const query = state.focusRunQuery.trim().toLowerCase();

  if (!query) {
    return mergedIds;
  }

  return mergedIds.filter((runId) => pinnedIds.includes(runId) || runId.toLowerCase().includes(query));
}

async function fetchHealth() {
  try {
    const response = await fetch('/api/health');
    const payload = await response.json();
    if (!response.ok) {
      throw new Error('服务状态检查失败');
    }
    state.health = payload;
    renderProviderStatus();
  } catch (_error) {
    state.health = null;
    renderProviderStatus();
  }
}

async function fetchRunDetail(runId) {
  if (!runId) {
    return null;
  }
  if (state.runDetails[runId]) {
    return state.runDetails[runId];
  }

  const response = await fetch(`/api/runs/${encodeURIComponent(runId)}`);
  if (!response.ok) {
    return null;
  }

  const payload = await response.json();
  state.runDetails[runId] = payload.run;
  return payload.run;
}

function renderSessionList() {
  elements.sessionList.innerHTML = '';
  renderSessionQuickSelect();

  if (state.sidebarView === 'runs' && state.selectedSession) {
    renderSessionRunList();
    return;
  }

  renderSessionDirectoryList();
}

function renderSessionQuickSelect() {
  elements.sessionQuickSelect.innerHTML = '';

  if (!state.sessions.length) {
    const option = document.createElement('option');
    option.value = '';
    option.textContent = '暂无会话';
    elements.sessionQuickSelect.appendChild(option);
    elements.sessionQuickSelect.disabled = true;
    return;
  }

  state.sessions.forEach((session) => {
    const option = document.createElement('option');
    option.value = session.id;
    option.textContent = session.title;
    option.selected = session.id === state.selectedSessionId;
    elements.sessionQuickSelect.appendChild(option);
  });
  elements.sessionQuickSelect.disabled = false;
}

function renderSidebarHeader() {
  const inRunView = state.sidebarView === 'runs' && state.selectedSession;
  elements.sidebarBackButton.hidden = !inRunView;
  elements.createSessionButton.hidden = inRunView;
  elements.sidebarKickerText.textContent = inRunView ? 'Runs' : 'Sessions';
  elements.sidebarTitleText.textContent = inRunView ? (state.selectedSession?.title || '当前会话') : '调试会话';
  elements.sessionCountText.textContent = inRunView
    ? `${getSessionRunIds().length} 个 run`
    : `${state.sessions.length} 个会话`;
}

function renderSessionDirectoryList() {
  renderSidebarHeader();

  if (!state.sessions.length) {
    elements.sessionList.innerHTML = '<p class="supporting-text">还没有调试会话。先新建一个，再把 run 纳入时间线。</p>';
    return;
  }

  state.sessions.forEach((session) => {
    const row = document.createElement('div');
    row.className = 'session-row';

    const button = document.createElement('button');
    button.type = 'button';
    button.className = `session-item ${session.id === state.selectedSessionId ? 'active' : ''}`;
    button.innerHTML = `
      <div class="run-item-title">
        <strong>${session.title}</strong>
        <span class="run-item-count">${session.runCount} runs</span>
      </div>
      <div class="run-item-meta">
        <span>${session.goal || '未填写调试目标'}</span>
      </div>
      <div class="run-item-meta">
        <span>${formatDateTime(session.updatedAt)}</span>
      </div>
    `;
    button.addEventListener('click', async () => {
      state.selectedSessionId = session.id;
      state.sidebarView = 'runs';
      saveSelectedDebugSessionId(session.id);
      await fetchSelectedSession();
    });

    const deleteButton = document.createElement('button');
    deleteButton.type = 'button';
    deleteButton.className = 'run-delete-icon';
    deleteButton.setAttribute('aria-label', `删除 ${session.title}`);
    deleteButton.innerHTML = createTrashIconMarkup();
    deleteButton.addEventListener('click', async (event) => {
      event.stopPropagation();
      try {
        await deleteSession(session.id, session.title);
      } catch (error) {
        window.alert(error instanceof Error ? error.message : '删除会话失败');
      }
    });

    row.appendChild(button);
    row.appendChild(deleteButton);
    elements.sessionList.appendChild(row);
  });
}

function renderSessionRunList() {
  renderSidebarHeader();

  const entries = [...(state.selectedSession?.entries || [])].reverse();
  if (!entries.length) {
    elements.sessionList.innerHTML = '<p class="supporting-text">当前会话还没有加入任何 run。可从主页或中间焦点区域加入。</p>';
    return;
  }

  entries.forEach((entry) => {
    const row = document.createElement('div');
    row.className = 'session-row';

    const button = document.createElement('button');
    button.type = 'button';
    button.className = `session-item session-run-item ${entry.runId === state.focusRunId ? 'active' : ''}`;
    button.innerHTML = `
      <div class="run-item-title">
        <strong>${entry.runId}</strong>
        <span class="run-item-count">${formatPercent(entry.run?.summary?.detectionCoverage)}</span>
      </div>
      <div class="run-item-meta">
        <span>误差 ${formatMeters(entry.run?.summary?.avgAbsDistanceError)}</span>
      </div>
      <div class="run-item-meta">
        <span>${formatDateTime(entry.addedAt)}</span>
      </div>
    `;
    button.addEventListener('click', async () => {
      state.focusRunId = entry.runId;
      const detail = await fetchRunDetail(entry.runId);
      renderSessionList();
      renderFocusRun(detail);
      renderChat();
    });

    row.appendChild(button);
    elements.sessionList.appendChild(row);
  });
}

function renderFocusRunSelect() {
  const optionIds = buildFocusRunOptionIds();
  elements.focusRunSelect.innerHTML = '';

  if (!optionIds.length) {
    const option = document.createElement('option');
    option.value = '';
    option.textContent = state.focusRunQuery ? '没有匹配的 run' : '暂无可用 run';
    elements.focusRunSelect.appendChild(option);
    elements.focusRunSelect.disabled = true;
    return;
  }

  optionIds.forEach((runId) => {
    const option = document.createElement('option');
    option.value = runId;
    const tags = [];
    if (sessionContainsRun(runId)) {
      tags.push('已在会话');
    } else if (state.requestedRunId === runId) {
      tags.push('当前查看');
    }
    option.textContent = tags.length ? `${runId} · ${tags.join(' / ')}` : runId;
    option.selected = runId === state.focusRunId;
    elements.focusRunSelect.appendChild(option);
  });
  elements.focusRunSelect.disabled = false;
}

function renderFocusRunSummary(run) {
  elements.focusRunSummaryText.innerHTML = '';

  if (!run) {
    elements.focusRunSummaryText.textContent = '焦点 run 的摘要会显示在这里。';
    return;
  }

  const simInfoEntries = Object.entries(run.simInfo || {});
  if (!simInfoEntries.length) {
    elements.focusRunSummaryText.textContent = '当前 run 未提供显式 SimInfo。';
    return;
  }

  const fragment = document.createDocumentFragment();
  simInfoEntries.slice(0, 10).forEach(([key, value]) => {
    const token = document.createElement('div');
    token.className = 'summary-token';

    const label = document.createElement('span');
    label.className = 'summary-token-label';
    label.textContent = key;

    const text = document.createElement('strong');
    text.className = 'summary-token-value';
    text.textContent = String(value);

    token.appendChild(label);
    token.appendChild(text);
    fragment.appendChild(token);
  });

  if (simInfoEntries.length > 10) {
    const moreToken = document.createElement('div');
    moreToken.className = 'summary-token summary-token-muted';
    moreToken.textContent = `其余 ${simInfoEntries.length - 10} 项参数已省略`;
    fragment.appendChild(moreToken);
  }

  elements.focusRunSummaryText.appendChild(fragment);
}

function renderFocusRun(run) {
  renderFocusRunSelect();

  if (!run) {
    elements.focusRunMetaText.textContent = '可以直接从全部仿真目录里搜索并切换焦点 run，也可以把它加入当前调试会话。';
    elements.focusRunImage.style.display = 'none';
    elements.focusRunImage.removeAttribute('src');
    elements.focusRunImageEmpty.style.display = 'grid';
    elements.focusRunCoverageText.textContent = '覆盖率 -';
    elements.focusRunErrorText.textContent = '误差 -';
    elements.focusRunUpdatedText.textContent = '更新时间 -';
    renderFocusRunSummary(null);
    elements.addCurrentRunButton.disabled = true;
    elements.addCurrentRunButton.textContent = '加入当前会话';
    return;
  }

  elements.focusRunMetaText.textContent = `${run.id} · ${formatDateTime(run.updatedAt)}`;
  elements.focusRunCoverageText.textContent = `覆盖率 ${formatPercent(run.summary?.detectionCoverage)}`;
  elements.focusRunErrorText.textContent = `误差 ${formatMeters(run.summary?.avgAbsDistanceError)}`;
  elements.focusRunUpdatedText.textContent = `更新时间 ${formatDateTime(run.updatedAt)}`;
  renderFocusRunSummary(run);

  const previewImage = run.previewImage?.url || run.images?.[0]?.url || null;
  if (previewImage) {
    elements.focusRunImage.style.display = 'block';
    elements.focusRunImage.src = previewImage;
    elements.focusRunImageEmpty.style.display = 'none';
  } else {
    elements.focusRunImage.style.display = 'none';
    elements.focusRunImage.removeAttribute('src');
    elements.focusRunImageEmpty.style.display = 'grid';
  }

  if (!state.selectedSessionId) {
    elements.addCurrentRunButton.disabled = false;
    elements.addCurrentRunButton.textContent = '创建会话并加入';
  } else if (sessionContainsRun(run.id)) {
    elements.addCurrentRunButton.disabled = false;
    elements.addCurrentRunButton.textContent = '更新该条目备注';
  } else {
    elements.addCurrentRunButton.disabled = false;
    elements.addCurrentRunButton.textContent = '加入当前会话';
  }
}

async function fetchRunsIndex() {
  const response = await fetch('/api/runs');
  const payload = await response.json();
  state.runs = payload.runs || [];

  if (state.focusRunId && !state.runs.some((run) => run.id === state.focusRunId)) {
    delete state.runDetails[state.focusRunId];
    state.focusRunId = null;
  }

  renderFocusRunSelect();
}

function renderTimeline() {
  elements.sessionTimeline.innerHTML = '';
  const entries = [...(state.selectedSession?.entries || [])].reverse();

  if (!entries.length) {
    elements.sessionTimeline.innerHTML = '<div class="timeline-empty">当前会话还没有 run。可以从主页带一个 run 过来，或从焦点区域加入当前 run。</div>';
    return;
  }

  entries.forEach((entry) => {
    const item = document.createElement('article');
    item.className = 'timeline-item';
    item.innerHTML = `
      <div class="timeline-item-head">
        <div>
          <h3>${entry.runId}</h3>
          <p class="supporting-text">${formatDateTime(entry.addedAt)}</p>
        </div>
        <div class="timeline-actions">
          <button class="text-button timeline-jump-button" type="button">设为焦点</button>
          <button class="entry-remove-icon" type="button" aria-label="移除 ${entry.runId}">${createTrashIconMarkup()}</button>
        </div>
      </div>
      <p class="timeline-metrics">覆盖率 ${formatPercent(entry.run?.summary?.detectionCoverage)} · 丢失率 ${formatPercent(entry.run?.summary?.lossRatio)} · 误差 ${formatMeters(entry.run?.summary?.avgAbsDistanceError)}</p>
      <div class="timeline-notes">
        <p><strong>调整说明：</strong>${entry.changeNote || '未记录'}</p>
        <p><strong>调整假设：</strong>${entry.hypothesis || '未记录'}</p>
        <p><strong>结果备注：</strong>${entry.resultNote || '未记录'}</p>
      </div>
    `;

    item.querySelector('.timeline-jump-button').addEventListener('click', async () => {
      state.focusRunId = entry.runId;
      const detail = await fetchRunDetail(entry.runId);
      renderFocusRun(detail);
      renderChat();
    });
    item.querySelector('.entry-remove-icon').addEventListener('click', async () => {
      try {
        await deleteSessionEntry(entry.id, entry.runId);
      } catch (error) {
        window.alert(error instanceof Error ? error.message : '移除条目失败');
      }
    });
    elements.sessionTimeline.appendChild(item);
  });
}

function renderSessionWorkspace() {
  if (!state.selectedSession) {
    elements.sessionTitleHeading.textContent = '未选择调试会话';
    elements.sessionMetaText.textContent = '先在左侧创建或选择一个调试会话，然后再把 run 纳入时间线。';
    elements.sessionTitleInput.value = '';
    elements.sessionGoalInput.value = '';
    elements.sessionThreadStatus.textContent = getSessionThreadStatusText();
    elements.sessionRunCountText.textContent = '0 个 run';
    elements.sessionMessageCountText.textContent = '0 条消息';
    elements.saveSessionMetaButton.disabled = true;
    elements.deleteCurrentSessionButton.disabled = true;
    renderTimeline();
    renderChat();
    return;
  }

  const summary = state.selectedSession.summary || {};
  elements.sessionTitleHeading.textContent = state.selectedSession.title || '未命名调试会话';
  elements.sessionMetaText.textContent = `${state.selectedSession.goal || '未填写调试目标'} · 更新于 ${formatDateTime(state.selectedSession.updatedAt)}`;
  elements.sessionTitleInput.value = state.selectedSession.title || '';
  elements.sessionGoalInput.value = state.selectedSession.goal || '';
  elements.sessionThreadStatus.textContent = getSessionThreadStatusText();
  elements.sessionRunCountText.textContent = `${summary.runCount ?? 0} 个 run`;
  elements.sessionMessageCountText.textContent = `${summary.messageCount ?? 0} 条消息`;
  elements.saveSessionMetaButton.disabled = false;
  elements.deleteCurrentSessionButton.disabled = false;
  renderTimeline();
  renderChat();
}

function renderChat() {
  elements.chatStream.innerHTML = '';
  elements.sendButton.textContent = getSendButtonText();

  if (!state.selectedSession) {
    elements.assistantSessionText.textContent = '请选择一个调试会话。';
    elements.chatStream.innerHTML = '<div class="chat-message meta">先在左侧选择或新建一个调试会话。</div>';
    return;
  }

  elements.assistantSessionText.textContent = state.focusRunId
    ? `当前会话：${state.selectedSession.title} · 当前焦点 run：${state.focusRunId}`
    : `当前会话：${state.selectedSession.title} · 请先指定一个焦点 run`;

  const messages = state.selectedSession.messages || [];
  if (!messages.length) {
    elements.chatStream.innerHTML = '<div class="chat-message meta">会话里还没有聊天记录。选择焦点 run 后可以开始连续对话。</div>';
    return;
  }

  messages.forEach((message) => {
    const role = message.role === 'assistant' || message.role === 'user' ? message.role : 'meta';
    const node = document.createElement('div');
    node.className = `chat-message ${role}`;
    node.textContent = message.content;
    elements.chatStream.appendChild(node);
  });
  elements.chatStream.scrollTop = elements.chatStream.scrollHeight;
}

async function syncFocusRun(preferredRunId) {
  const optionIds = dedupeRunIds([preferredRunId, state.requestedRunId, ...getSessionRunIds()]);
  state.focusRunId = optionIds[0] || null;
  const run = await fetchRunDetail(state.focusRunId);
  renderFocusRun(run);
}

async function fetchSessions() {
  const response = await fetch('/api/debug-sessions');
  const payload = await response.json();
  state.sessions = payload.sessions || [];

  if (state.selectedSessionId && !state.sessions.some((session) => session.id === state.selectedSessionId)) {
    state.selectedSessionId = state.sessions[0]?.id || null;
    saveSelectedDebugSessionId(state.selectedSessionId);
  }

  if (!state.selectedSessionId && state.sessions.length) {
    state.selectedSessionId = state.sessions[0].id;
    saveSelectedDebugSessionId(state.selectedSessionId);
  }

  renderSessionList();
  await fetchSelectedSession();
}

async function fetchSelectedSession() {
  if (!state.selectedSessionId) {
    state.selectedSession = null;
    state.sidebarView = 'sessions';
    renderSessionList();
    renderSessionWorkspace();
    await syncFocusRun(state.requestedRunId);
    return;
  }

  const response = await fetch(`/api/debug-sessions/${encodeURIComponent(state.selectedSessionId)}`);
  if (!response.ok) {
    state.selectedSession = null;
    state.sidebarView = 'sessions';
    renderSessionList();
    renderSessionWorkspace();
    await syncFocusRun(state.requestedRunId);
    return;
  }

  const payload = await response.json();
  state.selectedSession = payload.session;
  renderSessionList();
  renderSessionWorkspace();
  await syncFocusRun(state.focusRunId || state.requestedRunId || state.selectedSession.summary?.lastRunId);
}

async function createSession() {
  const defaultTitle = state.focusRunId ? `围绕 ${state.focusRunId} 的调试会话` : `调试会话 ${state.sessions.length + 1}`;
  const body = {
    title: elements.sessionTitleInput.value.trim() || defaultTitle,
    goal: elements.sessionGoalInput.value.trim(),
  };
  const response = await fetch('/api/debug-sessions', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(body),
  });
  const payload = await response.json();
  if (!response.ok) {
    throw new Error(payload.error || '新建会话失败');
  }

  state.selectedSessionId = payload.session.id;
  saveSelectedDebugSessionId(state.selectedSessionId);
  await fetchSessions();
}

async function saveSessionMeta() {
  if (!state.selectedSessionId) {
    await createSession();
    return;
  }

  const response = await fetch(`/api/debug-sessions/${encodeURIComponent(state.selectedSessionId)}`, {
    method: 'PATCH',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      title: elements.sessionTitleInput.value.trim(),
      goal: elements.sessionGoalInput.value.trim(),
    }),
  });
  const payload = await response.json();
  if (!response.ok) {
    throw new Error(payload.error || '保存会话失败');
  }

  state.selectedSession = payload.session;
  await fetchSessions();
}

async function deleteSession(sessionId, sessionTitle) {
  const confirmed = window.confirm(`确认删除调试会话 ${sessionTitle} 吗？会话时间线和聊天记录都会丢失。`);
  if (!confirmed) {
    return;
  }

  const response = await fetch(`/api/debug-sessions/${encodeURIComponent(sessionId)}`, {
    method: 'DELETE',
  });
  const payload = await response.json();
  if (!response.ok) {
    throw new Error(payload.error || '删除会话失败');
  }

  if (state.selectedSessionId === sessionId) {
    state.selectedSessionId = null;
    state.sidebarView = 'sessions';
    saveSelectedDebugSessionId(null);
  }
  await fetchSessions();
}

async function addCurrentRunToSession() {
  if (!state.focusRunId) {
    window.alert('请先指定一个焦点 run。');
    return;
  }
  if (!state.selectedSessionId) {
    await createSession();
  }

  const response = await fetch(`/api/debug-sessions/${encodeURIComponent(state.selectedSessionId)}/runs`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      runId: state.focusRunId,
      changeNote: elements.changeNoteInput.value.trim(),
      hypothesis: elements.hypothesisInput.value.trim(),
      resultNote: elements.resultNoteInput.value.trim(),
    }),
  });
  const payload = await response.json();
  if (!response.ok) {
    throw new Error(payload.error || '加入会话失败');
  }

  if (!payload.added) {
    window.alert('该 run 已存在于当前会话中，本次没有重复加入；如果填写了备注，则已更新到原条目。');
  }

  elements.changeNoteInput.value = '';
  elements.hypothesisInput.value = '';
  elements.resultNoteInput.value = '';
  state.selectedSession = payload.session;
  await fetchSessions();
}

async function deleteSessionEntry(entryId, runId) {
  const confirmed = window.confirm(`确认把 run ${runId} 从当前调试会话中移除吗？`);
  if (!confirmed) {
    return;
  }

  const response = await fetch(`/api/debug-sessions/${encodeURIComponent(state.selectedSessionId)}/runs/${encodeURIComponent(entryId)}`, {
    method: 'DELETE',
  });
  const payload = await response.json();
  if (!response.ok) {
    throw new Error(payload.error || '移除条目失败');
  }

  state.selectedSession = payload.session;
  if (state.focusRunId === runId && !sessionContainsRun(runId) && state.requestedRunId !== runId) {
    state.focusRunId = null;
  }
  await fetchSessions();
}

function getCurrentFocusImageName() {
  const run = state.focusRunId ? state.runDetails[state.focusRunId] : null;
  return run?.previewImage?.name || run?.images?.[0]?.name || null;
}

async function sendChatMessage(prompt) {
  if (!state.selectedSessionId) {
    window.alert('请先选择一个调试会话。');
    return;
  }
  if (!state.focusRunId) {
    window.alert('请先指定一个焦点 run。');
    return;
  }
  if (!SettingsStore.isConfigured(state.settings)) {
    window.alert('请先到设置页完成 AI 配置。');
    return;
  }

  const previousSession = state.selectedSession ? JSON.parse(JSON.stringify(state.selectedSession)) : null;
  const optimisticMessages = [...(state.selectedSession?.messages || [])];
  optimisticMessages.push({ role: 'user', content: prompt });
  optimisticMessages.push({ role: 'assistant', content: getActiveProviderBusyText() });
  state.selectedSession = {
    ...state.selectedSession,
    messages: optimisticMessages,
  };
  renderChat();
  elements.sendButton.disabled = true;

  try {
    const response = await fetch('/api/chat', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        debugSessionId: state.selectedSessionId,
        runId: state.focusRunId,
        imageName: getCurrentFocusImageName(),
        message: prompt,
        settings: state.settings,
      }),
    });
    const payload = await response.json();
    if (!response.ok) {
      throw new Error(payload.error || '聊天失败');
    }

    state.selectedSession = payload.session;
    await fetchSessions();
  } catch (error) {
    state.selectedSession = previousSession;
    renderChat();
    throw error;
  } finally {
    elements.sendButton.disabled = false;
  }
}

function openRunViewer() {
  if (!state.focusRunId) {
    window.alert('当前没有可查看的焦点 run。');
    return;
  }
  localStorage.setItem(RUN_SELECTION_STORAGE_KEY, state.focusRunId);
  window.location.href = `/?runId=${encodeURIComponent(state.focusRunId)}`;
}

function bindEvents() {
  elements.createSessionButton.addEventListener('click', async () => {
    try {
      await createSession();
    } catch (error) {
      window.alert(error instanceof Error ? error.message : '新建会话失败');
    }
  });

  elements.sidebarBackButton.addEventListener('click', () => {
    state.sidebarView = 'sessions';
    renderSessionList();
  });

  elements.sessionQuickSelect.addEventListener('change', async () => {
    state.selectedSessionId = elements.sessionQuickSelect.value || null;
    saveSelectedDebugSessionId(state.selectedSessionId);
    await fetchSelectedSession();
  });

  elements.saveSessionMetaButton.addEventListener('click', async () => {
    try {
      await saveSessionMeta();
    } catch (error) {
      window.alert(error instanceof Error ? error.message : '保存会话失败');
    }
  });

  elements.deleteCurrentSessionButton.addEventListener('click', async () => {
    if (!state.selectedSession) {
      return;
    }
    try {
      await deleteSession(state.selectedSession.id, state.selectedSession.title);
    } catch (error) {
      window.alert(error instanceof Error ? error.message : '删除会话失败');
    }
  });

  elements.addCurrentRunButton.addEventListener('click', async () => {
    try {
      await addCurrentRunToSession();
    } catch (error) {
      window.alert(error instanceof Error ? error.message : '加入会话失败');
    }
  });

  elements.focusRunSearchInput.addEventListener('input', () => {
    state.focusRunQuery = elements.focusRunSearchInput.value.trim();
    renderFocusRunSelect();
  });

  elements.focusRunSelect.addEventListener('change', async () => {
    state.focusRunId = elements.focusRunSelect.value || null;
    const detail = await fetchRunDetail(state.focusRunId);
    renderFocusRun(detail);
    renderChat();
  });

  elements.openRunViewerButton.addEventListener('click', openRunViewer);

  elements.chatForm.addEventListener('submit', async (event) => {
    event.preventDefault();
    const prompt = elements.chatInput.value.trim();
    if (!prompt) {
      return;
    }
    elements.chatInput.value = '';
    try {
      await sendChatMessage(prompt);
    } catch (error) {
      window.alert(error instanceof Error ? error.message : '聊天失败');
    }
  });

  window.addEventListener('storage', async (event) => {
    if (event.key === SettingsStore.STORAGE_KEY) {
      state.settings = SettingsStore.loadSettings();
      renderProviderStatus();
      renderSessionWorkspace();
    }
    if (event.key === DEBUG_SESSION_STORAGE_KEY) {
      state.selectedSessionId = loadSelectedDebugSessionId();
      state.sidebarView = state.selectedSessionId ? state.sidebarView : 'sessions';
      await fetchSessions();
    }
  });
}

function connectEvents() {
  const source = new EventSource('/api/events');

  source.addEventListener('hello', () => {
    setConnectionState(true, '服务已连接');
  });

  source.addEventListener('runs_updated', async () => {
    setConnectionState(true, '实时监听中');
    await fetchRunsIndex();
    if (state.focusRunId) {
      state.runDetails[state.focusRunId] = null;
    }
    await fetchSelectedSession();
  });

  source.onerror = () => {
    setConnectionState(false, '连接中断，等待重连');
  };
}

async function bootstrap() {
  renderProviderStatus();
  bindEvents();
  setConnectionState(false, '正在连接服务');
  await Promise.all([fetchHealth(), fetchRunsIndex()]);
  await fetchSessions();
  connectEvents();
}

bootstrap().catch((error) => {
  window.alert(error instanceof Error ? error.message : '初始化失败');
});
