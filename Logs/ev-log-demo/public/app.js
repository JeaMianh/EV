const SettingsStore = window.EVLogSettings;

const LAYOUT_STORAGE_KEY = 'ev-log-demo-layout';
const RUN_SELECTION_STORAGE_KEY = 'ev-log-demo-selected-run-id';
const DEBUG_SESSION_STORAGE_KEY = 'ev-log-demo-selected-debug-session';

const CHART_META = {
  speed: {
    title: '速度轨迹',
    subtitle: 'Ego vs Target',
    buildDatasets(run) {
      return [
        {
          label: 'EgoSpd',
          data: run?.series?.EgoSpd || [],
          borderColor: '#1565c0',
          backgroundColor: 'rgba(21, 101, 192, 0.12)',
          tension: 0.26,
          pointRadius: 0,
          borderWidth: 2.5,
        },
        {
          label: 'TgtSpd',
          data: run?.series?.TgtSpd || [],
          borderColor: '#ef6c00',
          backgroundColor: 'rgba(239, 108, 0, 0.10)',
          tension: 0.26,
          pointRadius: 0,
          borderWidth: 2.5,
        },
      ];
    },
  },
  distance: {
    title: '距离观测',
    subtitle: 'Ground Truth vs Radar',
    buildDatasets(run) {
      return [
        {
          label: 'GT_Dist',
          data: run?.series?.GT_Dist || [],
          borderColor: '#00897b',
          backgroundColor: 'rgba(0, 137, 123, 0.12)',
          tension: 0.24,
          pointRadius: 0,
          borderWidth: 2.5,
        },
        {
          label: 'Radar_Dist',
          data: run?.series?.Radar_Dist_Visible || [],
          borderColor: '#b26a00',
          backgroundColor: 'rgba(178, 106, 0, 0.10)',
          tension: 0.18,
          pointRadius: 0,
          spanGaps: true,
          borderWidth: 2.5,
        },
      ];
    },
  },
  status: {
    title: '跟踪状态',
    subtitle: 'Detection, Loss, Ghost',
    buildDatasets(run) {
      return [
        {
          label: 'Radar_Valid',
          data: run?.series?.Radar_Valid || [],
          borderColor: '#2e7d32',
          backgroundColor: 'rgba(46, 125, 50, 0.10)',
          stepped: true,
          pointRadius: 0,
          borderWidth: 2.4,
        },
        {
          label: 'Flag_Loss',
          data: run?.series?.Flag_Loss || [],
          borderColor: '#d32f2f',
          backgroundColor: 'rgba(211, 47, 47, 0.10)',
          stepped: true,
          pointRadius: 0,
          borderWidth: 2.4,
        },
        {
          label: 'Flag_Ghost',
          data: run?.series?.Flag_Ghost || [],
          borderColor: '#6d4c41',
          backgroundColor: 'rgba(109, 76, 65, 0.10)',
          stepped: true,
          pointRadius: 0,
          borderWidth: 2.4,
        },
      ];
    },
    yRange: {
      min: -0.05,
      max: 1.1,
      stepSize: 0.5,
    },
  },
};

function getRequestedRunId() {
  const queryRunId = new URLSearchParams(window.location.search).get('runId');
  return queryRunId || localStorage.getItem(RUN_SELECTION_STORAGE_KEY) || null;
}

function loadLayoutState() {
  try {
    const raw = localStorage.getItem(LAYOUT_STORAGE_KEY);
    const parsed = raw ? JSON.parse(raw) : {};
    return {
      leftCollapsed: Boolean(parsed.leftCollapsed),
    };
  } catch (_error) {
    return {
      leftCollapsed: false,
    };
  }
}

function saveLayoutState(layout) {
  localStorage.setItem(LAYOUT_STORAGE_KEY, JSON.stringify(layout));
}

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

const state = {
  requestedRunId: getRequestedRunId(),
  runs: [],
  selectedRunId: null,
  selectedRun: null,
  selectedImageName: null,
  settings: SettingsStore.loadSettings(),
  health: null,
  layout: loadLayoutState(),
  charts: {},
  expandedChartKey: null,
  expandedChart: null,
};

const elements = {
  body: document.body,
  connectionDot: document.querySelector('#connectionDot'),
  connectionText: document.querySelector('#connectionText'),
  providerStatusText: document.querySelector('#providerStatusText'),
  toggleLeftSidebarButton: document.querySelector('#toggleLeftSidebarButton'),
  collapseLeftSidebarButton: document.querySelector('#collapseLeftSidebarButton'),
  lastScanText: document.querySelector('#lastScanText'),
  runCountText: document.querySelector('#runCountText'),
  runList: document.querySelector('#runList'),
  runTitle: document.querySelector('#runTitle'),
  runSubtitle: document.querySelector('#runSubtitle'),
  openSessionsButton: document.querySelector('#openSessionsButton'),
  summaryGrid: document.querySelector('#summaryGrid'),
  imageMetaText: document.querySelector('#imageMetaText'),
  mainImage: document.querySelector('#mainImage'),
  imageEmpty: document.querySelector('#imageEmpty'),
  thumbList: document.querySelector('#thumbList'),
  chartModal: document.querySelector('#chartModal'),
  chartModalBackdrop: document.querySelector('#chartModalBackdrop'),
  closeChartModalButton: document.querySelector('#closeChartModalButton'),
  chartModalTitle: document.querySelector('#chartModalTitle'),
  chartModalSubtitle: document.querySelector('#chartModalSubtitle'),
  chartModalCanvas: document.querySelector('#chartModalCanvas'),
};

const summaryCards = [...elements.summaryGrid.querySelectorAll('.metric-card')];

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

function formatSeconds(value) {
  return Number.isFinite(value) ? `${value.toFixed(2)} s` : '-';
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

function createPlusIconMarkup() {
  return `
    <svg viewBox="0 0 24 24" aria-hidden="true">
      <path d="M11 5h2v6h6v2h-6v6h-2v-6H5v-2h6V5Z"></path>
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

function renderSidebarControls() {
  elements.body.classList.toggle('left-collapsed', state.layout.leftCollapsed);
  elements.toggleLeftSidebarButton.textContent = state.layout.leftCollapsed ? '展开目录' : '收起目录';
}

function updateLayout(partialLayout) {
  state.layout = {
    ...state.layout,
    ...partialLayout,
  };
  saveLayoutState(state.layout);
  renderSidebarControls();
}

function buildChartOptions(chartKey, expanded) {
  const chartMeta = CHART_META[chartKey];
  const yRange = chartMeta.yRange || {};
  return {
    responsive: true,
    maintainAspectRatio: false,
    interaction: {
      intersect: false,
      mode: 'index',
    },
    animation: false,
    plugins: {
      legend: {
        labels: {
          color: '#32404f',
          usePointStyle: true,
          boxWidth: expanded ? 14 : 10,
          font: {
            size: expanded ? 13 : 12,
          },
        },
      },
      tooltip: {
        backgroundColor: '#253344',
        titleColor: '#ffffff',
        bodyColor: '#ecf3ff',
        padding: 12,
      },
    },
    scales: {
      x: {
        ticks: {
          color: '#607080',
          maxTicksLimit: expanded ? 14 : 8,
        },
        grid: {
          color: 'rgba(143, 155, 168, 0.18)',
        },
      },
      y: {
        min: yRange.min,
        max: yRange.max,
        ticks: {
          color: '#607080',
          stepSize: yRange.stepSize,
        },
        grid: {
          color: 'rgba(143, 155, 168, 0.18)',
        },
      },
    },
  };
}

function createChart(canvas, chartKey, expanded = false) {
  return new Chart(canvas, {
    type: 'line',
    data: {
      labels: [],
      datasets: [],
    },
    options: buildChartOptions(chartKey, expanded),
  });
}

function applyChartData(chart, chartKey, run, expanded = false) {
  const labels = (run?.series?.Time || []).map((value) => Number(value || 0).toFixed(2));
  chart.data.labels = labels;
  chart.data.datasets = CHART_META[chartKey].buildDatasets(run);
  chart.options = buildChartOptions(chartKey, expanded);
  chart.update();
}

function initCharts() {
  state.charts.speed = createChart(document.querySelector('#speedChart'), 'speed');
  state.charts.distance = createChart(document.querySelector('#distanceChart'), 'distance');
  state.charts.status = createChart(document.querySelector('#statusChart'), 'status');
}

async function deleteRun(runId) {
  const confirmed = window.confirm(`确认删除日志目录 ${runId} 吗？此操作不可恢复。`);
  if (!confirmed) {
    return;
  }

  try {
    const response = await fetch(`/api/runs/${encodeURIComponent(runId)}`, {
      method: 'DELETE',
    });
    const payload = await response.json();
    if (!response.ok) {
      throw new Error(payload.error || '删除失败');
    }

    if (state.selectedRunId === runId) {
      state.selectedRunId = null;
      state.selectedRun = null;
      state.selectedImageName = null;
      localStorage.removeItem(RUN_SELECTION_STORAGE_KEY);
    }

    await fetchRuns();
  } catch (error) {
    window.alert(error instanceof Error ? error.message : '删除失败');
  }
}

async function createDebugSessionForRun(runId) {
  const response = await fetch('/api/debug-sessions', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      title: `围绕 ${runId} 的调试会话`,
      goal: '',
    }),
  });
  const payload = await response.json();
  if (!response.ok) {
    throw new Error(payload.error || '新建调试会话失败');
  }

  const sessionId = payload.session?.id || null;
  saveSelectedDebugSessionId(sessionId);
  return sessionId;
}

async function addRunToDebugSession(sessionId, runId) {
  return fetch(`/api/debug-sessions/${encodeURIComponent(sessionId)}/runs`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      runId,
      changeNote: '',
      hypothesis: '',
      resultNote: '',
    }),
  });
}

async function addRunToCurrentDebugSession(runId) {
  let sessionId = loadSelectedDebugSessionId();
  if (!sessionId) {
    sessionId = await createDebugSessionForRun(runId);
  }

  let response = await addRunToDebugSession(sessionId, runId);
  let payload = await response.json();

  if (response.status === 404) {
    sessionId = await createDebugSessionForRun(runId);
    response = await addRunToDebugSession(sessionId, runId);
    payload = await response.json();
  }

  if (!response.ok) {
    throw new Error(payload.error || '加入调试会话失败');
  }

  saveSelectedDebugSessionId(sessionId);
  return {
    sessionId,
    added: payload.added !== false,
  };
}

function renderRunList() {
  elements.runCountText.textContent = `${state.runs.length} runs`;
  elements.runList.innerHTML = '';

  if (!state.runs.length) {
    elements.runList.innerHTML = '<p class="supporting-text">还没有发现可解析的仿真目录。</p>';
    return;
  }

  state.runs.forEach((run) => {
    const row = document.createElement('div');
    row.className = 'run-row';

    const button = document.createElement('button');
    button.type = 'button';
    button.className = `run-item ${run.id === state.selectedRunId ? 'active' : ''}`;
    button.innerHTML = `
      <div class="run-item-title">
        <strong>${run.id}</strong>
        <span class="run-item-count">${run.imageCount} 图</span>
      </div>
      <div class="run-item-meta">
        <span>覆盖率 ${formatPercent(run.summary?.detectionCoverage)}</span>
        <span>误差 ${formatMeters(run.summary?.avgAbsDistanceError).replace(' m', '')}</span>
      </div>
      <div class="run-item-meta">
        <span>${formatDateTime(run.updatedAt)}</span>
      </div>
    `;
    button.addEventListener('click', () => {
      selectRun(run.id);
    });

    const deleteButton = document.createElement('button');
    deleteButton.type = 'button';
    deleteButton.className = 'run-delete-icon';
    deleteButton.setAttribute('aria-label', `删除 ${run.id}`);
    deleteButton.innerHTML = createTrashIconMarkup();
    deleteButton.addEventListener('click', (event) => {
      event.stopPropagation();
      deleteRun(run.id);
    });

    const addButton = document.createElement('button');
    addButton.type = 'button';
    addButton.className = 'run-add-icon';
    addButton.setAttribute('aria-label', `把 ${run.id} 加入当前调试会话`);
    addButton.title = '加入当前调试会话';
    addButton.innerHTML = createPlusIconMarkup();
    addButton.addEventListener('click', async (event) => {
      event.stopPropagation();
      addButton.disabled = true;
      try {
        const result = await addRunToCurrentDebugSession(run.id);
        window.alert(result.added ? `已将 ${run.id} 加入当前调试会话。` : `${run.id} 已存在于当前调试会话中。`);
      } catch (error) {
        window.alert(error instanceof Error ? error.message : '加入调试会话失败');
      } finally {
        addButton.disabled = false;
      }
    });

    row.appendChild(button);
    row.appendChild(deleteButton);
    row.appendChild(addButton);
    elements.runList.appendChild(row);
  });
}

function renderSummary(run) {
  const cards = [
    ['仿真时长', formatSeconds(run?.summary?.durationSeconds)],
    ['检测覆盖率', formatPercent(run?.summary?.detectionCoverage)],
    ['平均距离误差', formatMeters(run?.summary?.avgAbsDistanceError)],
    ['目标丢失比例', formatPercent(run?.summary?.lossRatio)],
  ];

  summaryCards.forEach((card, index) => {
    const [label, value] = cards[index];
    card.querySelector('.metric-label').textContent = label;
    card.querySelector('.metric-value').textContent = value;
  });
}

function renderRunHeader(run) {
  if (!run) {
    elements.runTitle.textContent = '等待选择日志';
    elements.runSubtitle.textContent = '主页只负责查看本地仿真数据。调试过程编排和 AI 连续对话请转到“调试会话”页。';
    elements.openSessionsButton.disabled = true;
    return;
  }

  const settingText = run.simInfo
    ? Object.entries(run.simInfo)
        .map(([key, value]) => `${key}=${value}`)
        .join(' | ')
    : '无显式 SimInfo';

  elements.runTitle.textContent = run.id;
  elements.runSubtitle.textContent = `更新于 ${formatDateTime(run.updatedAt)} · ${settingText}`;
  elements.openSessionsButton.disabled = false;
}

function renderCharts(run) {
  applyChartData(state.charts.speed, 'speed', run);
  applyChartData(state.charts.distance, 'distance', run);
  applyChartData(state.charts.status, 'status', run);

  if (state.expandedChartKey) {
    renderExpandedChart();
  }
}

function renderImages(run) {
  const images = run?.images || [];
  const activeImage = images.find((image) => image.name === state.selectedImageName) || images[0] || null;
  state.selectedImageName = activeImage?.name || null;

  elements.thumbList.innerHTML = '';
  elements.imageMetaText.textContent = activeImage ? activeImage.name : '暂无图片';

  if (activeImage) {
    elements.mainImage.style.display = 'block';
    elements.mainImage.src = activeImage.url;
    elements.imageEmpty.style.display = 'none';
  } else {
    elements.mainImage.style.display = 'none';
    elements.mainImage.removeAttribute('src');
    elements.imageEmpty.style.display = 'grid';
  }

  images.forEach((image) => {
    const button = document.createElement('button');
    button.type = 'button';
    button.className = `thumb-button ${image.name === state.selectedImageName ? 'active' : ''}`;
    button.textContent = image.name;
    button.addEventListener('click', () => {
      state.selectedImageName = image.name;
      renderImages(run);
    });
    elements.thumbList.appendChild(button);
  });
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

async function fetchRuns() {
  const response = await fetch('/api/runs');
  const payload = await response.json();
  state.runs = payload.runs || [];
  elements.lastScanText.textContent = payload.lastScannedAt ? `最近扫描：${formatDateTime(payload.lastScannedAt)}` : '尚未完成扫描';

  const requestedRunStillExists = state.requestedRunId && state.runs.some((run) => run.id === state.requestedRunId);
  if (!state.selectedRunId) {
    state.selectedRunId = requestedRunStillExists ? state.requestedRunId : state.runs[0]?.id || null;
  }

  if (state.selectedRunId && !state.runs.some((run) => run.id === state.selectedRunId)) {
    state.selectedRunId = state.runs[0]?.id || null;
  }

  renderRunList();

  if (state.selectedRunId) {
    await fetchRun(state.selectedRunId);
  } else {
    state.selectedRun = null;
    renderRunHeader(null);
    renderSummary(null);
    renderCharts(null);
    renderImages(null);
  }
}

async function fetchRun(runId) {
  const response = await fetch(`/api/runs/${encodeURIComponent(runId)}`);
  if (!response.ok) {
    throw new Error('加载仿真详情失败。');
  }

  const payload = await response.json();
  state.selectedRun = payload.run;
  localStorage.setItem(RUN_SELECTION_STORAGE_KEY, runId);
  renderRunHeader(state.selectedRun);
  renderSummary(state.selectedRun);
  renderCharts(state.selectedRun);
  renderImages(state.selectedRun);
  renderRunList();
}

async function selectRun(runId) {
  if (runId === state.selectedRunId && state.selectedRun) {
    return;
  }

  state.selectedRunId = runId;
  state.selectedImageName = null;
  renderRunList();
  await fetchRun(runId);
}

function openSessionsPage() {
  if (!state.selectedRunId) {
    window.alert('请先选择一个仿真目录。');
    return;
  }
  localStorage.setItem(RUN_SELECTION_STORAGE_KEY, state.selectedRunId);
  window.location.href = `/sessions?runId=${encodeURIComponent(state.selectedRunId)}`;
}

function renderExpandedChart() {
  const chartKey = state.expandedChartKey;
  if (!chartKey) {
    return;
  }

  const chartMeta = CHART_META[chartKey];
  elements.chartModalTitle.textContent = chartMeta.title;
  elements.chartModalSubtitle.textContent = chartMeta.subtitle;

  if (state.expandedChart) {
    state.expandedChart.destroy();
  }

  state.expandedChart = createChart(elements.chartModalCanvas, chartKey, true);
  applyChartData(state.expandedChart, chartKey, state.selectedRun, true);
}

function openExpandedChart(chartKey) {
  state.expandedChartKey = chartKey;
  elements.chartModal.hidden = false;
  document.body.classList.add('modal-open');
  renderExpandedChart();
}

function closeExpandedChart() {
  state.expandedChartKey = null;
  elements.chartModal.hidden = true;
  document.body.classList.remove('modal-open');
  if (state.expandedChart) {
    state.expandedChart.destroy();
    state.expandedChart = null;
  }
}

function bindEvents() {
  elements.toggleLeftSidebarButton.addEventListener('click', () => {
    updateLayout({ leftCollapsed: !state.layout.leftCollapsed });
  });
  elements.collapseLeftSidebarButton.addEventListener('click', () => {
    updateLayout({ leftCollapsed: true });
  });
  elements.openSessionsButton.addEventListener('click', openSessionsPage);

  elements.closeChartModalButton.addEventListener('click', closeExpandedChart);
  elements.chartModalBackdrop.addEventListener('click', closeExpandedChart);

  document.addEventListener('keydown', (event) => {
    if (event.key === 'Escape' && !elements.chartModal.hidden) {
      closeExpandedChart();
    }
  });

  document.querySelectorAll('[data-expand-chart]').forEach((button) => {
    button.addEventListener('click', () => {
      openExpandedChart(button.dataset.expandChart);
    });
  });

  window.addEventListener('storage', (event) => {
    if (event.key === SettingsStore.STORAGE_KEY) {
      state.settings = SettingsStore.loadSettings();
      renderProviderStatus();
    }
    if (event.key === LAYOUT_STORAGE_KEY) {
      state.layout = loadLayoutState();
      renderSidebarControls();
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
    await fetchRuns();
  });

  source.onerror = () => {
    setConnectionState(false, '连接中断，等待重连');
  };
}

async function bootstrap() {
  renderSidebarControls();
  renderProviderStatus();
  initCharts();
  renderRunHeader(null);
  renderSummary(null);
  renderImages(null);
  bindEvents();
  setConnectionState(false, '正在连接服务');
  await Promise.all([fetchHealth(), fetchRuns()]);
  connectEvents();
}

bootstrap().catch((error) => {
  window.alert(error instanceof Error ? error.message : '初始化失败');
});
