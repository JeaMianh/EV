const STORAGE_KEYS = {
  apiConfig: "simlog-monitor-api-config-v1",
  chatHistory: "simlog-monitor-chat-history-v1",
};

const CHART_COLORS = ["#8fe3b0", "#6ec2ff", "#ffc97d", "#ff8a80", "#c8a8ff"];

const state = {
  meta: null,
  runs: [],
  selectedRunId: "",
  selectedRunDetail: null,
  selectedImageIndex: 0,
  apiConfig: loadApiConfig(),
  chatHistory: loadChatHistory(),
  eventSource: null,
};

const elements = {
  watchRoot: document.querySelector("#watch-root"),
  connectionStatus: document.querySelector("#connection-status"),
  runList: document.querySelector("#run-list"),
  runTitle: document.querySelector("#run-title"),
  runStatus: document.querySelector("#run-status"),
  runUpdatedAt: document.querySelector("#run-updated-at"),
  metricStrip: document.querySelector("#metric-strip"),
  insightList: document.querySelector("#insight-list"),
  suggestionList: document.querySelector("#suggestion-list"),
  parseNotice: document.querySelector("#parse-notice"),
  chartGrid: document.querySelector("#chart-grid"),
  imageViewer: document.querySelector("#image-viewer"),
  imageThumbs: document.querySelector("#image-thumbs"),
  seriesTable: document.querySelector("#series-table"),
  chatMessages: document.querySelector("#chat-messages"),
  chatForm: document.querySelector("#chat-form"),
  chatInput: document.querySelector("#chat-input"),
  sendButton: document.querySelector("#send-button"),
  apiForm: document.querySelector("#api-config-form"),
  refreshRuns: document.querySelector("#refresh-runs"),
};

boot().catch((error) => {
  console.error(error);
  addSystemMessage(`初始化失败：${error instanceof Error ? error.message : "Unknown error"}`);
});

async function boot() {
  bindConfigForm();
  bindChatForm();
  bindPromptButtons();
  elements.refreshRuns.addEventListener("click", () => refreshRuns(true));
  hydrateConfigForm();
  await refreshMeta();
  await refreshRuns(false);
  connectEvents();
}

async function refreshMeta() {
  const meta = await fetchJson("/api/meta");
  state.meta = meta;
  elements.watchRoot.textContent = meta.watchRoot;
}

async function refreshRuns(preserveSelection = true) {
  const payload = await fetchJson("/api/runs");
  state.runs = payload.items;
  renderRunList();

  if (!state.runs.length) {
    state.selectedRunId = "";
    state.selectedRunDetail = null;
    renderDetail();
    return;
  }

  const keepCurrent = preserveSelection && state.runs.some((item) => item.id === state.selectedRunId);
  const nextId = keepCurrent ? state.selectedRunId : state.runs[0].id;
  await selectRun(nextId);
}

async function selectRun(runId) {
  if (!runId) {
    return;
  }

  state.selectedRunId = runId;
  state.selectedImageIndex = 0;
  renderRunList();
  state.selectedRunDetail = await fetchJson(`/api/runs/${encodeURIComponent(runId)}`);
  renderDetail();
  renderChat();
}

function renderRunList() {
  if (!state.runs.length) {
    elements.runList.innerHTML = `<div class="empty-state">目录中暂时没有可展示的仿真结果。</div>`;
    return;
  }

  elements.runList.innerHTML = state.runs
    .map((run) => {
      const isActive = run.id === state.selectedRunId;
      const summary = run.summary || {};
      const duration = formatMetric(summary.durationSeconds, "s");
      const minDistance = formatMetric(summary.minDistance, "m");
      const radarRatio =
        summary.radarAvailabilityRatio === null || summary.radarAvailabilityRatio === undefined
          ? "n/a"
          : `${(summary.radarAvailabilityRatio * 100).toFixed(0)}%`;

      return `
        <button class="run-item ${isActive ? "active" : ""}" data-run-id="${escapeHtml(run.id)}" type="button">
          <h3>${escapeHtml(run.title)}</h3>
          <div class="run-meta-row">
            <span class="mini-chip">${escapeHtml(run.status)}</span>
            <span class="mini-chip">${escapeHtml(duration)}</span>
            <span class="mini-chip">${escapeHtml(minDistance)}</span>
            <span class="mini-chip">radar ${escapeHtml(radarRatio)}</span>
          </div>
          <p>${escapeHtml(run.insightHeadline || "无额外摘要，可点击查看详情。")}</p>
          <p>${escapeHtml(formatDate(run.updatedAt))}</p>
        </button>
      `;
    })
    .join("");

  for (const button of elements.runList.querySelectorAll("[data-run-id]")) {
    button.addEventListener("click", () => {
      selectRun(button.dataset.runId).catch((error) => {
        addSystemMessage(`切换运行失败：${error instanceof Error ? error.message : "Unknown error"}`);
      });
    });
  }
}

function renderDetail() {
  const run = state.selectedRunDetail;

  if (!run) {
    elements.runTitle.textContent = "选择一条运行记录";
    elements.runStatus.textContent = "等待选择";
    elements.runUpdatedAt.textContent = "--";
    elements.metricStrip.innerHTML = "";
    elements.insightList.innerHTML = `<div class="empty-state">暂无数据。</div>`;
    elements.suggestionList.innerHTML = `<div class="empty-state">暂无建议。</div>`;
    elements.parseNotice.textContent = "";
    elements.chartGrid.innerHTML = `<div class="panel chart-card"><div class="chart-empty">选择左侧运行后，这里会出现依据 SimLog.json 绘制的图表。</div></div>`;
    elements.imageViewer.innerHTML = `<div class="image-empty">暂无图片。</div>`;
    elements.imageThumbs.innerHTML = "";
    elements.seriesTable.innerHTML = `<div class="empty-state">暂无序列。</div>`;
    renderChat();
    return;
  }

  elements.runTitle.textContent = run.title;
  elements.runStatus.textContent = run.status;
  elements.runUpdatedAt.textContent = formatDate(run.updatedAt);
  elements.metricStrip.innerHTML = buildMetricCards(run.summary || {});
  elements.insightList.innerHTML = buildStackList(run.insights || []);
  elements.suggestionList.innerHTML = buildStackList((run.suggestions || []).map((text) => ({ level: "info", title: text })));
  elements.parseNotice.textContent = run.parseNotice || "";
  elements.parseNotice.style.display = run.parseNotice ? "block" : "none";
  elements.chartGrid.innerHTML = buildChartsMarkup(run.charts || []);
  elements.imageViewer.innerHTML = buildImageViewer(run.images || []);
  elements.imageThumbs.innerHTML = buildImageThumbs(run.images || []);
  elements.seriesTable.innerHTML = buildSeriesTable(run.seriesIndex || []);
  attachThumbHandlers();
}

function attachThumbHandlers() {
  const run = state.selectedRunDetail;
  for (const button of elements.imageThumbs.querySelectorAll("[data-image-index]")) {
    button.addEventListener("click", () => {
      state.selectedImageIndex = Number(button.dataset.imageIndex || 0);
      elements.imageViewer.innerHTML = buildImageViewer(run.images || []);
      elements.imageThumbs.innerHTML = buildImageThumbs(run.images || []);
      attachThumbHandlers();
    });
  }
}

function buildMetricCards(summary) {
  const metrics = [
    ["样本数", summary.sampleCount ?? "n/a"],
    ["时长", formatMetric(summary.durationSeconds, "s")],
    ["最小距离", formatMetric(summary.minDistance, "m")],
    ["Radar 有效率", summary.radarAvailabilityRatio == null ? "n/a" : `${(summary.radarAvailabilityRatio * 100).toFixed(1)}%`],
    ["平均距离误差", formatMetric(summary.meanDistanceError, "m")],
    ["Ego 最高速", formatMetric(summary.egoSpeedMax, "m/s")],
  ];

  return metrics
    .map(
      ([label, value]) => `
        <div class="metric-card">
          <span class="metric-label">${escapeHtml(String(label))}</span>
          <span class="metric-value">${escapeHtml(String(value))}</span>
        </div>
      `,
    )
    .join("");
}

function buildStackList(items) {
  if (!items.length) {
    return `<div class="empty-state">暂无内容。</div>`;
  }

  return items
    .map((item) => {
      const detail = item.detail || "";
      return `
        <div class="stack-item ${escapeHtml(item.level || "info")}">
          <strong>${escapeHtml(item.title || "")}</strong>
          ${detail ? `<span>${escapeHtml(detail)}</span>` : ""}
        </div>
      `;
    })
    .join("");
}

function buildChartsMarkup(charts) {
  if (!charts.length) {
    return `<div class="panel chart-card"><div class="chart-empty">这条运行缺少可绘制的时序数据。</div></div>`;
  }

  return charts
    .map((chart) => {
      const legend = chart.series
        .map(
          (series, index) => `
            <span class="legend-item">
              <span class="legend-swatch" style="background:${CHART_COLORS[index % CHART_COLORS.length]}"></span>
              ${escapeHtml(series.label)}
            </span>
          `,
        )
        .join("");

      return `
        <section class="panel chart-card">
          <div class="panel-head">
            <div>
              <p class="panel-kicker">Chart</p>
              <h2>${escapeHtml(chart.title)}</h2>
            </div>
          </div>
          <div class="chart-canvas">${buildChartSvg(chart)}</div>
          <div class="chart-legend">${legend}</div>
        </section>
      `;
    })
    .join("");
}

function buildChartSvg(chart) {
  const time = chart.time || [];
  const seriesCollection = chart.series || [];

  if (time.length < 2 || !seriesCollection.length) {
    return `<div class="chart-empty">数据不足，无法绘图。</div>`;
  }

  const width = 720;
  const height = 280;
  const padding = { left: 56, right: 16, top: 16, bottom: 34 };
  const innerWidth = width - padding.left - padding.right;
  const innerHeight = height - padding.top - padding.bottom;
  const values = seriesCollection.flatMap((series) => series.values.filter((item) => Number.isFinite(item)));

  if (!values.length) {
    return `<div class="chart-empty">没有有效数值。</div>`;
  }

  let minY = Math.min(...values);
  let maxY = Math.max(...values);
  if (minY === maxY) {
    minY -= 1;
    maxY += 1;
  }

  const minX = time[0];
  const maxX = time[time.length - 1];
  const xSpan = maxX - minX || 1;
  const ySpan = maxY - minY || 1;

  const toX = (value) => padding.left + ((value - minX) / xSpan) * innerWidth;
  const toY = (value) => padding.top + innerHeight - ((value - minY) / ySpan) * innerHeight;

  const horizontalGrid = Array.from({ length: 5 }, (_unused, index) => {
    const ratio = index / 4;
    const y = padding.top + innerHeight * ratio;
    const label = (maxY - ySpan * ratio).toFixed(2);
    return `
      <line class="grid-line" x1="${padding.left}" y1="${y}" x2="${width - padding.right}" y2="${y}" />
      <text class="grid-label" x="8" y="${y + 4}">${escapeHtml(label)}</text>
    `;
  }).join("");

  const verticalLabels = [time[0], time[Math.floor(time.length / 2)], time[time.length - 1]]
    .map((value, index) => {
      const x =
        index === 0
          ? padding.left
          : index === 1
            ? padding.left + innerWidth / 2
            : width - padding.right;
      return `<text class="axis-label" x="${x - 12}" y="${height - 8}">${escapeHtml(value.toFixed(2))}</text>`;
    })
    .join("");

  const paths = seriesCollection
    .map((series, index) => {
      const points = series.values
        .map((value, pointIndex) => `${toX(time[pointIndex]).toFixed(2)},${toY(value).toFixed(2)}`)
        .join(" ");
      return `<polyline fill="none" stroke="${CHART_COLORS[index % CHART_COLORS.length]}" stroke-width="2.6" points="${points}" />`;
    })
    .join("");

  return `
    <svg class="chart-svg" viewBox="0 0 ${width} ${height}" role="img" aria-label="${escapeHtml(chart.title)}">
      ${horizontalGrid}
      <line class="grid-line" x1="${padding.left}" y1="${height - padding.bottom}" x2="${width - padding.right}" y2="${height - padding.bottom}" />
      ${paths}
      ${verticalLabels}
    </svg>
  `;
}

function buildImageViewer(images) {
  if (!images.length) {
    return `<div class="image-empty">当前运行没有可显示的分析图片。</div>`;
  }

  const image = images[state.selectedImageIndex] || images[0];
  return `<img src="${escapeAttribute(image.publicUrl)}" alt="${escapeAttribute(image.name)}" />`;
}

function buildImageThumbs(images) {
  if (images.length <= 1) {
    return "";
  }

  return images
    .map(
      (image, index) => `
        <button class="thumb-button ${index === state.selectedImageIndex ? "active" : ""}" data-image-index="${index}" type="button">
          ${escapeHtml(image.name)}
        </button>
      `,
    )
    .join("");
}

function buildSeriesTable(seriesIndex) {
  if (!seriesIndex.length) {
    return `<div class="empty-state">没有可展示的序列。</div>`;
  }

  return seriesIndex
    .map(
      (item) => `
        <div class="series-row">
          <strong>${escapeHtml(item.key)}</strong>
          <span>min ${escapeHtml(formatValue(item.min))}</span>
          <span>max ${escapeHtml(formatValue(item.max))}</span>
          <span>last ${escapeHtml(formatValue(item.last))}</span>
        </div>
      `,
    )
    .join("");
}

function renderChat() {
  const runId = state.selectedRunId || "__global__";
  const messages = state.chatHistory[runId] || [];

  if (!messages.length) {
    elements.chatMessages.innerHTML = `
      <div class="chat-bubble system">
        当前没有聊天记录。选中一条运行后，可以直接让助手解释图片、诊断时序曲线，或者给出下一轮实验建议。
      </div>
    `;
    return;
  }

  elements.chatMessages.innerHTML = messages
    .map(
      (message) => `
        <div class="chat-bubble ${escapeHtml(message.role)}">${escapeHtml(message.content)}</div>
      `,
    )
    .join("");

  elements.chatMessages.scrollTop = elements.chatMessages.scrollHeight;
}

function bindConfigForm() {
  elements.apiForm.addEventListener("input", () => {
    state.apiConfig = {
      baseUrl: document.querySelector("#api-base-url").value.trim(),
      apiKey: document.querySelector("#api-key").value.trim(),
      model: document.querySelector("#api-model").value.trim(),
      systemPrompt: document.querySelector("#api-system-prompt").value.trim(),
      includeImages: document.querySelector("#include-images").checked,
    };
    localStorage.setItem(STORAGE_KEYS.apiConfig, JSON.stringify(state.apiConfig));
  });
}

function hydrateConfigForm() {
  document.querySelector("#api-base-url").value = state.apiConfig.baseUrl;
  document.querySelector("#api-key").value = state.apiConfig.apiKey;
  document.querySelector("#api-model").value = state.apiConfig.model;
  document.querySelector("#api-system-prompt").value = state.apiConfig.systemPrompt;
  document.querySelector("#include-images").checked = state.apiConfig.includeImages;
}

function bindChatForm() {
  elements.chatForm.addEventListener("submit", async (event) => {
    event.preventDefault();
    const prompt = elements.chatInput.value.trim();
    if (!prompt) {
      return;
    }

    await sendPrompt(prompt);
    elements.chatInput.value = "";
  });
}

function bindPromptButtons() {
  for (const button of document.querySelectorAll("[data-prompt]")) {
    button.addEventListener("click", async () => {
      await sendPrompt(button.dataset.prompt || "");
    });
  }
}

async function sendPrompt(prompt) {
  if (!state.selectedRunId) {
    addSystemMessage("请先在左侧选择一条运行记录。");
    return;
  }

  if (!state.apiConfig.baseUrl || !state.apiConfig.apiKey || !state.apiConfig.model) {
    addSystemMessage("请先填写 Base URL、API Key 和 Model。");
    return;
  }

  const runId = state.selectedRunId;
  const history = state.chatHistory[runId] || [];
  const nextMessages = [...history, { role: "user", content: prompt }];
  state.chatHistory[runId] = nextMessages;
  persistChatHistory();
  renderChat();
  elements.sendButton.disabled = true;
  elements.sendButton.textContent = "发送中...";

  try {
    const payload = await fetchJson("/api/chat", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        runId,
        apiConfig: state.apiConfig,
        messages: nextMessages,
      }),
    });

    state.chatHistory[runId] = [...state.chatHistory[runId], { role: "assistant", content: payload.reply }];
    persistChatHistory();
    renderChat();
  } catch (error) {
    addSystemMessage(`聊天请求失败：${error instanceof Error ? error.message : "Unknown error"}`);
  } finally {
    elements.sendButton.disabled = false;
    elements.sendButton.textContent = "发送";
  }
}

function addSystemMessage(content) {
  const runId = state.selectedRunId || "__global__";
  const history = state.chatHistory[runId] || [];
  state.chatHistory[runId] = [...history, { role: "system", content }];
  persistChatHistory();
  renderChat();
}

function persistChatHistory() {
  localStorage.setItem(STORAGE_KEYS.chatHistory, JSON.stringify(state.chatHistory));
}

function connectEvents() {
  if (state.eventSource) {
    state.eventSource.close();
  }

  const eventSource = new EventSource("/api/events");
  state.eventSource = eventSource;

  eventSource.onopen = () => {
    elements.connectionStatus.textContent = "SSE 已连接";
  };

  eventSource.onmessage = async (event) => {
    const payload = JSON.parse(event.data);

    if (payload.type === "runs-updated" || payload.type === "run-removed") {
      await refreshRuns(true);
    }
  };

  eventSource.onerror = () => {
    elements.connectionStatus.textContent = "SSE 已断开，重连中";
  };
}

async function fetchJson(url, options) {
  const response = await fetch(url, options);

  if (!response.ok) {
    const maybeJson = await tryReadJson(response);
    throw new Error(maybeJson?.detail || maybeJson?.error || response.statusText);
  }

  return response.json();
}

async function tryReadJson(response) {
  try {
    return await response.json();
  } catch {
    return null;
  }
}

function loadApiConfig() {
  const fallback = {
    baseUrl: "https://api.openai.com/v1",
    apiKey: "",
    model: "gpt-4.1-mini",
    systemPrompt: "",
    includeImages: true,
  };

  const stored = localStorage.getItem(STORAGE_KEYS.apiConfig);
  if (!stored) {
    return fallback;
  }

  try {
    return { ...fallback, ...JSON.parse(stored) };
  } catch {
    return fallback;
  }
}

function loadChatHistory() {
  const stored = localStorage.getItem(STORAGE_KEYS.chatHistory);
  if (!stored) {
    return {};
  }

  try {
    return JSON.parse(stored) || {};
  } catch {
    return {};
  }
}

function formatMetric(value, unit) {
  if (value === null || value === undefined || Number.isNaN(value)) {
    return "n/a";
  }
  return `${Number(value).toFixed(value >= 100 ? 0 : value >= 10 ? 1 : 2)}${unit}`;
}

function formatValue(value) {
  if (value === null || value === undefined || Number.isNaN(value)) {
    return "n/a";
  }
  return String(value);
}

function formatDate(value) {
  const date = new Date(value);
  if (Number.isNaN(date.getTime())) {
    return value || "n/a";
  }
  return date.toLocaleString("zh-CN", { hour12: false });
}

function escapeHtml(value) {
  return String(value)
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#39;");
}

function escapeAttribute(value) {
  return escapeHtml(value);
}
