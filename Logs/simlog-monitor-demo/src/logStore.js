import fs from "node:fs/promises";
import path from "node:path";
import chokidar from "chokidar";

const IMAGE_EXTENSIONS = new Set([".png", ".jpg", ".jpeg", ".webp"]);
const IMAGE_MIME_TYPES = {
  ".png": "image/png",
  ".jpg": "image/jpeg",
  ".jpeg": "image/jpeg",
  ".webp": "image/webp",
};

const PRESET_DEFINITIONS = [
  {
    id: "speed",
    title: "Speed Profile",
    description: "Ego 与 Target 速度变化。",
    keys: ["EgoSpd", "TgtSpd"],
  },
  {
    id: "distance",
    title: "Distance Tracking",
    description: "Ground truth 与 radar 距离对比。",
    keys: ["GT_Dist", "Radar_Dist"],
  },
  {
    id: "radar",
    title: "Radar Status",
    description: "相对速度与跟踪状态量。",
    keys: ["Radar_RelVel", "Target_ID"],
  },
  {
    id: "flags",
    title: "Flags",
    description: "虚警、丢失等离散状态变化。",
    keys: ["Flag_Ghost", "Flag_Loss"],
  },
];

export class LogStore {
  constructor({ watchRoot, projectRoot }) {
    this.watchRoot = path.resolve(watchRoot);
    this.projectRoot = path.resolve(projectRoot);
    this.projectDirName = path.basename(this.projectRoot);
    this.runs = new Map();
    this.listeners = new Set();
    this.pendingScans = new Map();
    this.watcher = null;
  }

  async initialize() {
    const entries = await fs.readdir(this.watchRoot, { withFileTypes: true });

    for (const entry of entries) {
      if (!entry.isDirectory() || this.shouldIgnoreDirectoryName(entry.name)) {
        continue;
      }

      await this.scanDirectory(entry.name);
    }
  }

  startWatching() {
    if (this.watcher) {
      return;
    }

    this.watcher = chokidar.watch(this.watchRoot, {
      ignoreInitial: true,
      depth: 2,
      awaitWriteFinish: {
        stabilityThreshold: 700,
        pollInterval: 100,
      },
      ignored: (watchedPath) => this.shouldIgnorePath(watchedPath),
    });

    this.watcher.on("all", (_eventName, changedPath) => {
      const directoryName = this.resolveDirectoryName(changedPath);

      if (!directoryName || this.shouldIgnoreDirectoryName(directoryName)) {
        return;
      }

      this.scheduleScan(directoryName);
    });
  }

  subscribe(listener) {
    this.listeners.add(listener);
    return () => this.listeners.delete(listener);
  }

  getRunList() {
    return [...this.runs.values()]
      .map((run) => ({
        id: run.id,
        title: run.title,
        updatedAt: run.updatedAt,
        status: run.status,
        summary: run.summary,
        insightHeadline: run.insights[0]?.title || "",
        imageCount: run.files.images.length,
        primaryImageUrl: run.files.images[0]?.publicUrl || "",
      }))
      .sort((left, right) => right.updatedAt.localeCompare(left.updatedAt));
  }

  getRunDetail(runId) {
    const run = this.runs.get(runId);
    return run ? sanitizeRun(run) : null;
  }

  getRunInternal(runId) {
    return this.runs.get(runId) || null;
  }

  async scanDirectory(directoryName) {
    const absoluteDirectory = path.join(this.watchRoot, directoryName);
    const run = await buildRunRecord({
      absoluteDirectory,
      directoryName,
    }).catch((error) => {
      return {
        id: directoryName,
        title: directoryName,
        updatedAt: new Date().toISOString(),
        status: "parse_error",
        available: {
          hasSimLog: false,
          hasImages: false,
          hasMat: false,
        },
        simInfo: {},
        summary: {
          durationSeconds: 0,
          sampleCount: 0,
          validSeriesCount: 0,
        },
        charts: [],
        rawKeys: [],
        insights: [
          {
            level: "warn",
            title: "目录解析失败",
            detail: error instanceof Error ? error.message : "Unknown error",
          },
        ],
        suggestions: ["先确认日志文件已经写完，再刷新页面。"],
        files: {
          simLog: null,
          images: [],
          mats: [],
        },
        parseNotice: error instanceof Error ? error.message : "Unknown error",
        seriesIndex: [],
      };
    });

    if (!run) {
      if (this.runs.delete(directoryName)) {
        this.emit({
          type: "run-removed",
          runId: directoryName,
          timestamp: new Date().toISOString(),
        });
      }
      return;
    }

    this.runs.set(directoryName, run);
    this.emit({
      type: "runs-updated",
      runId: directoryName,
      timestamp: new Date().toISOString(),
    });
  }

  shouldIgnorePath(candidatePath) {
    const absolutePath = path.resolve(candidatePath);
    return (
      absolutePath.startsWith(this.projectRoot) ||
      absolutePath.includes(`${path.sep}node_modules${path.sep}`)
    );
  }

  shouldIgnoreDirectoryName(directoryName) {
    return !directoryName || directoryName === this.projectDirName || directoryName === "node_modules";
  }

  resolveDirectoryName(candidatePath) {
    const relativePath = path.relative(this.watchRoot, path.resolve(candidatePath));

    if (!relativePath || relativePath.startsWith("..")) {
      return null;
    }

    return relativePath.split(path.sep)[0];
  }

  scheduleScan(directoryName) {
    clearTimeout(this.pendingScans.get(directoryName));

    const timer = setTimeout(() => {
      this.pendingScans.delete(directoryName);
      this.scanDirectory(directoryName).catch(() => {});
    }, 400);

    this.pendingScans.set(directoryName, timer);
  }

  emit(payload) {
    for (const listener of this.listeners) {
      listener(payload);
    }
  }
}

async function buildRunRecord({ absoluteDirectory, directoryName }) {
  const directoryStat = await fs.stat(absoluteDirectory).catch(() => null);

  if (!directoryStat?.isDirectory()) {
    return null;
  }

  const entries = await fs.readdir(absoluteDirectory, { withFileTypes: true });
  const files = entries.filter((entry) => entry.isFile());
  const simLogEntry = files.find((entry) => entry.name.toLowerCase() === "simlog.json");
  const imageEntries = files.filter((entry) => IMAGE_EXTENSIONS.has(path.extname(entry.name).toLowerCase()));
  const matEntries = files.filter((entry) => path.extname(entry.name).toLowerCase() === ".mat");

  if (!simLogEntry && !imageEntries.length && !matEntries.length) {
    return null;
  }

  const fileMeta = await buildFileMeta({
    absoluteDirectory,
    directoryName,
    simLogEntry,
    imageEntries,
    matEntries,
  });

  const logResult = simLogEntry
    ? await parseSimLog(path.join(absoluteDirectory, simLogEntry.name))
    : { status: "image_only", data: null, parseNotice: "No SimLog.json found." };

  const normalized = logResult.data ? normalizeLogData(logResult.data) : createEmptyNormalizedData();
  const summary = buildSummary(normalized, logResult.status);
  const insights = buildInsights(normalized, summary, logResult.status, fileMeta);
  const suggestions = buildSuggestions(normalized, summary, logResult.status);
  const charts = buildCharts(normalized);
  const updatedAt = newestTimestamp(fileMeta, directoryStat.mtime.toISOString(), logResult.updatedAt);

  return {
    id: directoryName,
    title: directoryName,
    updatedAt,
    status: logResult.status,
    available: {
      hasSimLog: Boolean(simLogEntry),
      hasImages: imageEntries.length > 0,
      hasMat: matEntries.length > 0,
    },
    simInfo: normalized.simInfo,
    summary,
    charts,
    rawKeys: normalized.rawKeys,
    insights,
    suggestions,
    files: fileMeta,
    parseNotice: logResult.parseNotice,
    seriesIndex: normalized.seriesIndex,
  };
}

async function buildFileMeta({ absoluteDirectory, directoryName, simLogEntry, imageEntries, matEntries }) {
  const simLog = simLogEntry
    ? await createFileDescriptor({
        absolutePath: path.join(absoluteDirectory, simLogEntry.name),
        directoryName,
        name: simLogEntry.name,
      })
    : null;

  const images = [];
  for (const entry of imageEntries.sort((left, right) => left.name.localeCompare(right.name))) {
    const descriptor = await createFileDescriptor({
      absolutePath: path.join(absoluteDirectory, entry.name),
      directoryName,
      name: entry.name,
    });

    descriptor.mimeType = IMAGE_MIME_TYPES[path.extname(entry.name).toLowerCase()] || "application/octet-stream";
    descriptor.publicUrl = `/api/assets/${encodeURIComponent(directoryName)}/${encodeURIComponent(entry.name)}`;
    images.push(descriptor);
  }

  const mats = [];
  for (const entry of matEntries.sort((left, right) => left.name.localeCompare(right.name))) {
    mats.push(
      await createFileDescriptor({
        absolutePath: path.join(absoluteDirectory, entry.name),
        directoryName,
        name: entry.name,
      }),
    );
  }

  return {
    simLog,
    images,
    mats,
  };
}

async function createFileDescriptor({ absolutePath, directoryName, name }) {
  const stat = await fs.stat(absolutePath);
  return {
    directoryName,
    name,
    absolutePath,
    sizeBytes: stat.size,
    updatedAt: stat.mtime.toISOString(),
  };
}

async function parseSimLog(absolutePath) {
  const rawText = await fs.readFile(absolutePath, "utf8");

  try {
    const data = JSON.parse(rawText);
    const stat = await fs.stat(absolutePath);
    return {
      status: "ready",
      data,
      parseNotice: "",
      updatedAt: stat.mtime.toISOString(),
    };
  } catch (error) {
    return {
      status: "parse_error",
      data: null,
      parseNotice: error instanceof Error ? error.message : "Invalid JSON",
      updatedAt: new Date().toISOString(),
    };
  }
}

function normalizeLogData(rawData) {
  const rawKeys = Object.keys(rawData || {});
  const simInfo = isPlainObject(rawData?.SimInfo) ? rawData.SimInfo : {};
  const numericSeries = {};

  for (const key of rawKeys) {
    if (key === "SimInfo") {
      continue;
    }

    const maybeArray = sanitizeNumericArray(rawData[key]);

    if (maybeArray.length) {
      numericSeries[key] = maybeArray;
    }
  }

  let time = numericSeries.Time || [];
  if (!time.length) {
    const fallbackLength = Math.max(
      0,
      ...Object.entries(numericSeries)
        .filter(([key]) => key !== "Time")
        .map(([, values]) => values.length),
    );
    time = Array.from({ length: fallbackLength }, (_unused, index) => index);
  }

  const alignedSeries = {};
  for (const [key, values] of Object.entries(numericSeries)) {
    if (key === "Time") {
      continue;
    }

    const targetLength = Math.min(values.length, time.length);
    if (!targetLength) {
      continue;
    }
    alignedSeries[key] = values.slice(0, targetLength);
  }

  const alignedTime = time.slice(0, Math.max(0, ...Object.values(alignedSeries).map((values) => values.length), time.length));
  const metrics = computeMetrics(alignedTime, alignedSeries);

  return {
    rawKeys,
    simInfo,
    time: alignedTime,
    series: alignedSeries,
    seriesIndex: Object.entries(alignedSeries)
      .map(([key, values]) => ({
        key,
        count: values.length,
        min: roundNumber(min(values)),
        max: roundNumber(max(values)),
        last: roundNumber(values[values.length - 1]),
      }))
      .sort((left, right) => left.key.localeCompare(right.key)),
    metrics,
  };
}

function createEmptyNormalizedData() {
  return {
    rawKeys: [],
    simInfo: {},
    time: [],
    series: {},
    seriesIndex: [],
    metrics: {
      durationSeconds: 0,
      sampleCount: 0,
      egoSpeedMax: null,
      targetSpeedMax: null,
      minDistance: null,
      radarAvailabilityRatio: null,
      lossRatio: null,
      ghostRatio: null,
      meanDistanceError: null,
      maxDistanceError: null,
      radarSentinelValue: null,
    },
  };
}

function computeMetrics(time, series) {
  const egoSpeed = series.EgoSpd || [];
  const targetSpeed = series.TgtSpd || [];
  const gtDistance = series.GT_Dist || [];
  const radarDistance = series.Radar_Dist || [];
  const lossFlag = series.Flag_Loss || [];
  const ghostFlag = series.Flag_Ghost || [];
  const radarSentinelValue = inferRadarSentinel(radarDistance);
  const validRadarIndices = radarDistance
    .map((value, index) => ({ value, index }))
    .filter(({ value }) => Number.isFinite(value) && (radarSentinelValue === null || value !== radarSentinelValue))
    .map(({ index }) => index);

  const distanceErrors = validRadarIndices
    .map((index) => {
      if (!Number.isFinite(gtDistance[index]) || !Number.isFinite(radarDistance[index])) {
        return null;
      }
      return Math.abs(gtDistance[index] - radarDistance[index]);
    })
    .filter((value) => Number.isFinite(value));

  return {
    durationSeconds: time.length > 1 ? roundNumber(time[time.length - 1] - time[0]) : 0,
    sampleCount: time.length,
    egoSpeedMax: valueOrNull(max(egoSpeed)),
    targetSpeedMax: valueOrNull(max(targetSpeed)),
    minDistance: valueOrNull(min(gtDistance.length ? gtDistance : radarDistance)),
    radarAvailabilityRatio: radarDistance.length ? roundNumber(validRadarIndices.length / radarDistance.length) : null,
    lossRatio: lossFlag.length ? roundNumber(mean(lossFlag)) : null,
    ghostRatio: ghostFlag.length ? roundNumber(mean(ghostFlag)) : null,
    meanDistanceError: distanceErrors.length ? roundNumber(mean(distanceErrors)) : null,
    maxDistanceError: distanceErrors.length ? roundNumber(max(distanceErrors)) : null,
    radarSentinelValue,
  };
}

function buildSummary(normalized, status) {
  return {
    durationSeconds: normalized.metrics.durationSeconds,
    sampleCount: normalized.metrics.sampleCount,
    validSeriesCount: Object.keys(normalized.series).length,
    status,
    egoSpeedMax: normalized.metrics.egoSpeedMax,
    targetSpeedMax: normalized.metrics.targetSpeedMax,
    minDistance: normalized.metrics.minDistance,
    radarAvailabilityRatio: normalized.metrics.radarAvailabilityRatio,
    lossRatio: normalized.metrics.lossRatio,
    ghostRatio: normalized.metrics.ghostRatio,
    meanDistanceError: normalized.metrics.meanDistanceError,
  };
}

function buildInsights(normalized, summary, status, fileMeta) {
  const insights = [];

  if (status === "image_only") {
    insights.push({
      level: "info",
      title: "仅检测到图片或 MAT 文件",
      detail: "当前目录没有 SimLog.json，前端会展示图片，但无法生成基于时序数据的折线图。",
    });
  }

  if (status === "parse_error") {
    insights.push({
      level: "warn",
      title: "SimLog.json 还未可读",
      detail: "JSON 可能仍在写入，或者内容暂时不完整。服务会继续监听后续更新。",
    });
  }

  if (!fileMeta.images.length) {
    insights.push({
      level: "info",
      title: "当前目录没有分析图片",
      detail: "前端图片区会为空，但图表仍会根据 SimLog.json 直接绘制。",
    });
  }

  if (summary.durationSeconds > 0 && summary.durationSeconds < 1) {
    insights.push({
      level: "warn",
      title: "仿真时长很短",
      detail: `总时长约 ${summary.durationSeconds}s，调参依据可能不足。`,
    });
  }

  if (summary.sampleCount === 1 && status === "ready") {
    insights.push({
      level: "info",
      title: "当前日志只有单帧快照",
      detail: "可以查看当下状态，但还不足以判断完整动态趋势。若要画出更稳定的曲线，建议导出连续时序样本。",
    });
  }

  if (summary.radarAvailabilityRatio !== null && summary.radarAvailabilityRatio < 0.2) {
    insights.push({
      level: "warn",
      title: "Radar 有效量测占比偏低",
      detail: `Radar_Dist 有效样本占比约 ${(summary.radarAvailabilityRatio * 100).toFixed(1)}%，怀疑目标大部分时间未被稳定检测。`,
    });
  }

  if (summary.lossRatio !== null && summary.lossRatio > 0.5) {
    insights.push({
      level: "warn",
      title: "目标丢失比例偏高",
      detail: `Flag_Loss 平均值约 ${summary.lossRatio.toFixed(2)}，说明丢失标志在多数采样点被置位。`,
    });
  }

  if (summary.ghostRatio !== null && summary.ghostRatio > 0.05) {
    insights.push({
      level: "warn",
      title: "存在可见虚警",
      detail: `Flag_Ghost 平均值约 ${summary.ghostRatio.toFixed(2)}，需要检查 clutter 抑制或目标关联逻辑。`,
    });
  }

  if (summary.meanDistanceError !== null && summary.meanDistanceError > 10) {
    insights.push({
      level: "warn",
      title: "Radar 距离与 Ground Truth 偏差偏大",
      detail: `平均绝对误差约 ${summary.meanDistanceError}m，可能涉及标定、筛选门限或目标匹配。`,
    });
  }

  if (summary.egoSpeedMax !== null && summary.egoSpeedMax < 1 && (summary.targetSpeedMax || 0) > 10) {
    insights.push({
      level: "info",
      title: "场景主要在激励目标车而非自车",
      detail: "如果后续要验证控制器或相对运动估计，建议补充更多 Ego 动态变化场景。",
    });
  }

  if (!insights.length) {
    insights.push({
      level: "info",
      title: "当前运行没有明显结构性异常",
      detail: "基础数据完整，可以继续通过右侧聊天面板做更细的分析和追问。",
    });
  }

  return insights;
}

function buildSuggestions(normalized, summary, status) {
  const suggestions = [];

  if (status === "image_only") {
    suggestions.push("后续导出阶段优先补上 SimLog.json，前端才能直接绘制时序曲线并支持更细粒度诊断。");
  }

  if (summary.radarAvailabilityRatio !== null && summary.radarAvailabilityRatio < 0.2) {
    suggestions.push("检查目标是否长期超出 radar FOV、遮挡模型是否过于严格，或 `Radar_Dist = 200` 是否被用作无检测哨兵值。");
  }

  if (summary.lossRatio !== null && summary.lossRatio > 0.5) {
    suggestions.push("复核 track management 的建立/保持/删除门限，尤其是首次捕获和短时失锁后的重建逻辑。");
  }

  if (summary.meanDistanceError !== null && summary.meanDistanceError > 10) {
    suggestions.push("比较 `GT_Dist` 与 `Radar_Dist` 的误差随时间变化，区分固定偏置、量测跳变还是目标关联错误。");
  }

  if (summary.durationSeconds > 0 && summary.durationSeconds < 1) {
    suggestions.push("延长仿真窗口，至少覆盖目标进入视场、稳定跟踪和退出视场三个阶段。");
  }

  if (summary.sampleCount === 1 && status === "ready") {
    suggestions.push("如果这是调试快照，建议再额外导出最近几秒的连续样本，便于判断问题是瞬时现象还是系统性趋势。");
  }

  if (!suggestions.length) {
    suggestions.push("可以继续增加更多状态量导出，例如 yaw rate、lane geometry、track confidence，便于后续 AI 解释更可靠。");
  }

  return suggestions;
}

function buildCharts(normalized) {
  const usedKeys = new Set();
  const charts = [];

  for (const preset of PRESET_DEFINITIONS) {
    const chart = createChartFromKeys(normalized, preset, usedKeys);
    if (chart) {
      charts.push(chart);
    }
  }

  const remainingKeys = Object.keys(normalized.series).filter((key) => !usedKeys.has(key));
  if (remainingKeys.length) {
    const previewKeys = remainingKeys.slice(0, 3);
    const extraChart = createChartFromKeys(
      normalized,
      {
        id: "extra",
        title: "Additional Signals",
        description: "未归类但可绘图的剩余状态量。",
        keys: previewKeys,
      },
      usedKeys,
    );

    if (extraChart) {
      charts.push(extraChart);
    }
  }

  return charts;
}

function createChartFromKeys(normalized, preset, usedKeys) {
  const availableKeys = preset.keys.filter((key) => Array.isArray(normalized.series[key]) && normalized.series[key].length);

  if (!availableKeys.length || !normalized.time.length) {
    return null;
  }

  const sampled = downsample(normalized.time, availableKeys.map((key) => normalized.series[key]));

  availableKeys.forEach((key) => usedKeys.add(key));

  return {
    id: preset.id,
    title: preset.title,
    description: preset.description,
    time: sampled.time,
    series: availableKeys.map((key, index) => ({
      key,
      label: key,
      values: sampled.series[index],
    })),
  };
}

function downsample(time, seriesCollection, maxPoints = 320) {
  if (time.length <= maxPoints) {
    return { time, series: seriesCollection };
  }

  const stride = Math.ceil(time.length / maxPoints);
  const indices = [];

  for (let index = 0; index < time.length; index += stride) {
    indices.push(index);
  }

  if (indices[indices.length - 1] !== time.length - 1) {
    indices.push(time.length - 1);
  }

  return {
    time: indices.map((index) => time[index]),
    series: seriesCollection.map((series) => indices.map((index) => series[index])),
  };
}

function sanitizeRun(run) {
  return {
    id: run.id,
    title: run.title,
    updatedAt: run.updatedAt,
    status: run.status,
    available: run.available,
    simInfo: run.simInfo,
    summary: run.summary,
    charts: run.charts,
    rawKeys: run.rawKeys,
    insights: run.insights,
    suggestions: run.suggestions,
    parseNotice: run.parseNotice,
    seriesIndex: run.seriesIndex,
    images: run.files.images.map(({ name, publicUrl, sizeBytes, updatedAt }) => ({
      name,
      publicUrl,
      sizeBytes,
      updatedAt,
    })),
    mats: run.files.mats.map(({ name, sizeBytes, updatedAt }) => ({
      name,
      sizeBytes,
      updatedAt,
    })),
  };
}

function sanitizeNumericArray(value) {
  if (typeof value === "number") {
    return Number.isFinite(value) ? [value] : [];
  }

  if (typeof value === "string") {
    const numericValue = Number(value);
    return Number.isFinite(numericValue) ? [numericValue] : [];
  }

  if (!Array.isArray(value)) {
    return [];
  }

  return value
    .map((item) => (typeof item === "number" ? item : Number(item)))
    .filter((item) => Number.isFinite(item));
}

function inferRadarSentinel(values) {
  if (!values.length) {
    return null;
  }

  const exact200Count = values.filter((value) => value === 200).length;
  return exact200Count >= values.length * 0.1 ? 200 : null;
}

function newestTimestamp(fileMeta, directoryUpdatedAt, logUpdatedAt) {
  const candidates = [
    directoryUpdatedAt,
    logUpdatedAt,
    fileMeta.simLog?.updatedAt,
    ...fileMeta.images.map((image) => image.updatedAt),
    ...fileMeta.mats.map((file) => file.updatedAt),
  ].filter(Boolean);

  return candidates.sort().at(-1) || new Date().toISOString();
}

function isPlainObject(value) {
  return value !== null && typeof value === "object" && !Array.isArray(value);
}

function min(values) {
  return values.length ? Math.min(...values) : null;
}

function max(values) {
  return values.length ? Math.max(...values) : null;
}

function mean(values) {
  return values.length ? values.reduce((sum, value) => sum + value, 0) / values.length : null;
}

function roundNumber(value) {
  if (!Number.isFinite(value)) {
    return null;
  }

  return Number(value.toFixed(3));
}

function valueOrNull(value) {
  return Number.isFinite(value) ? roundNumber(value) : null;
}
