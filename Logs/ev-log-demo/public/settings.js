const SettingsStore = window.EVLogSettings;

const state = {
  settings: SettingsStore.loadSettings(),
  health: null,
};

const elements = {
  settingsForm: document.querySelector('#settingsForm'),
  providerTypeInput: document.querySelector('#providerTypeInput'),
  baseUrlInput: document.querySelector('#baseUrlInput'),
  apiKeyInput: document.querySelector('#apiKeyInput'),
  modelInput: document.querySelector('#modelInput'),
  remoteSettingsGroup: document.querySelector('#remoteSettingsGroup'),
  localCliInfoBlock: document.querySelector('#localCliInfoBlock'),
  includeImageInput: document.querySelector('#includeImageInput'),
  providerPreviewText: document.querySelector('#providerPreviewText'),
  imagePolicyText: document.querySelector('#imagePolicyText'),
  saveStatusText: document.querySelector('#saveStatusText'),
  resetSettingsButton: document.querySelector('#resetSettingsButton'),
  healthStatusText: document.querySelector('#healthStatusText'),
  logsRootText: document.querySelector('#logsRootText'),
  projectRootText: document.querySelector('#projectRootText'),
  runCountHealthText: document.querySelector('#runCountHealthText'),
  debugSessionCountText: document.querySelector('#debugSessionCountText'),
  codexStatusText: document.querySelector('#codexStatusText'),
  qwenStatusText: document.querySelector('#qwenStatusText'),
  iflowStatusText: document.querySelector('#iflowStatusText'),
};

function syncInputs() {
  elements.providerTypeInput.value = state.settings.providerType;
  elements.baseUrlInput.value = state.settings.baseUrl;
  elements.apiKeyInput.value = state.settings.apiKey;
  elements.modelInput.value = state.settings.model;
  elements.includeImageInput.checked = state.settings.includeImage;
}

function renderProviderSections() {
  const remoteMode = state.settings.providerType === 'openai_compatible';
  elements.remoteSettingsGroup.hidden = !remoteMode;
  elements.localCliInfoBlock.hidden = remoteMode;
}

function renderPreview() {
  if (state.settings.providerType !== 'openai_compatible') {
    const providerStatus = state.health?.localProviders?.[state.settings.providerType];
    const providerLabel = SettingsStore.getProviderLabel(state.settings);
    elements.providerPreviewText.textContent = providerStatus?.available
      ? `${providerLabel} 已连接`
      : `${providerLabel} 未检测到`;
  } else {
    elements.providerPreviewText.textContent = SettingsStore.isConfigured(state.settings) ? '远程 API 已配置' : '远程 API 未配置';
  }

  elements.imagePolicyText.textContent = state.settings.includeImage ? '开启' : '关闭';
}

function saveSettingsFromInputs() {
  state.settings = SettingsStore.sanitizeSettings({
    providerType: elements.providerTypeInput.value,
    baseUrl: elements.baseUrlInput.value,
    apiKey: elements.apiKeyInput.value,
    model: elements.modelInput.value,
    includeImage: elements.includeImageInput.checked,
  });
  SettingsStore.saveSettings(state.settings);
  renderProviderSections();
  renderPreview();
  elements.saveStatusText.textContent = `已自动保存 ${new Intl.DateTimeFormat('zh-CN', { timeStyle: 'medium' }).format(new Date())}`;
}

function resetSettings() {
  state.settings = SettingsStore.resetSettings();
  syncInputs();
  renderProviderSections();
  renderPreview();
  elements.saveStatusText.textContent = '已恢复默认配置。';
}

function renderProviderAvailability(element, providerStatus) {
  if (!providerStatus) {
    element.textContent = '未知';
    return;
  }
  element.textContent = providerStatus.available ? `可用 (${providerStatus.command})` : '未检测到';
}

async function fetchHealth() {
  try {
    const response = await fetch('/api/health');
    const payload = await response.json();
    if (!response.ok) {
      throw new Error('服务状态检查失败');
    }

    state.health = payload;
    elements.healthStatusText.textContent = payload.ok ? '正常' : '异常';
    elements.logsRootText.textContent = payload.logsRoot || '-';
    elements.projectRootText.textContent = payload.projectRoot || '-';
    elements.runCountHealthText.textContent = String(payload.runCount ?? '-');
    elements.debugSessionCountText.textContent = String(payload.debugSessionCount ?? '-');
    renderProviderAvailability(elements.codexStatusText, payload.localProviders?.codex_cli);
    renderProviderAvailability(elements.qwenStatusText, payload.localProviders?.qwen_cli);
    renderProviderAvailability(elements.iflowStatusText, payload.localProviders?.iflow_cli);
    renderPreview();
  } catch (_error) {
    state.health = null;
    elements.healthStatusText.textContent = '不可用';
    elements.logsRootText.textContent = '-';
    elements.projectRootText.textContent = '-';
    elements.runCountHealthText.textContent = '-';
    elements.debugSessionCountText.textContent = '-';
    elements.codexStatusText.textContent = '不可用';
    elements.qwenStatusText.textContent = '不可用';
    elements.iflowStatusText.textContent = '不可用';
    renderPreview();
  }
}

function bindEvents() {
  elements.settingsForm.addEventListener('input', saveSettingsFromInputs);
  elements.resetSettingsButton.addEventListener('click', resetSettings);

  window.addEventListener('storage', (event) => {
    if (event.key !== SettingsStore.STORAGE_KEY) {
      return;
    }

    state.settings = SettingsStore.loadSettings();
    syncInputs();
    renderProviderSections();
    renderPreview();
  });
}

async function bootstrap() {
  syncInputs();
  renderProviderSections();
  renderPreview();
  bindEvents();
  await fetchHealth();
}

bootstrap();
