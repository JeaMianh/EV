(function attachSettingsStore() {
  const STORAGE_KEY = 'ev-log-demo-settings';
  const DEFAULT_SETTINGS = {
    providerType: 'codex_cli',
    baseUrl: 'https://api.openai.com/v1',
    apiKey: '',
    model: '',
    includeImage: true,
  };

  const PROVIDER_LABELS = {
    codex_cli: 'Codex CLI',
    qwen_cli: 'Qwen Code',
    iflow_cli: 'iFlow CLI',
    openai_compatible: '远程 API',
  };

  const VALID_PROVIDER_TYPES = new Set(['codex_cli', 'qwen_cli', 'iflow_cli', 'openai_compatible']);

  function sanitizeSettings(rawSettings) {
    const providerType = VALID_PROVIDER_TYPES.has(rawSettings?.providerType)
      ? rawSettings.providerType
      : DEFAULT_SETTINGS.providerType;

    return {
      providerType,
      baseUrl: String(rawSettings?.baseUrl || DEFAULT_SETTINGS.baseUrl).trim(),
      apiKey: String(rawSettings?.apiKey || DEFAULT_SETTINGS.apiKey).trim(),
      model: String(rawSettings?.model || DEFAULT_SETTINGS.model).trim(),
      includeImage: rawSettings?.includeImage !== false,
    };
  }

  function loadSettings() {
    try {
      const raw = localStorage.getItem(STORAGE_KEY);
      const parsed = raw ? JSON.parse(raw) : {};
      return sanitizeSettings(parsed);
    } catch (_error) {
      return { ...DEFAULT_SETTINGS };
    }
  }

  function saveSettings(settings) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(sanitizeSettings(settings)));
  }

  function resetSettings() {
    const nextSettings = { ...DEFAULT_SETTINGS };
    saveSettings(nextSettings);
    return nextSettings;
  }

  function isConfigured(settings) {
    if (settings.providerType !== 'openai_compatible') {
      return true;
    }

    return Boolean(settings.baseUrl && settings.apiKey && settings.model);
  }

  function getProviderLabel(settings) {
    return PROVIDER_LABELS[settings.providerType] || 'AI Provider';
  }

  window.EVLogSettings = {
    STORAGE_KEY,
    DEFAULT_SETTINGS,
    PROVIDER_LABELS,
    sanitizeSettings,
    loadSettings,
    saveSettings,
    resetSettings,
    isConfigured,
    getProviderLabel,
  };
})();
