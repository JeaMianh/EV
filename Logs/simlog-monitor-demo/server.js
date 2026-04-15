import express from "express";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { LogStore } from "./src/logStore.js";
import {
  buildContextBlock,
  buildMessagesForApi,
  buildSystemPrompt,
  extractAssistantText,
  normalizeMessages,
} from "./src/prompt.js";

const __filename = fileURLToPath(import.meta.url);
const projectRoot = path.dirname(__filename);
const watchRoot = path.resolve(projectRoot, "..");
const publicDir = path.join(projectRoot, "public");
const port = Number(process.env.PORT || 3760);

const store = new LogStore({ watchRoot, projectRoot });
await store.initialize();
store.startWatching();

const app = express();

app.use(express.json({ limit: "25mb" }));
app.use(express.static(publicDir));

app.get("/api/meta", (_req, res) => {
  res.json({
    projectName: path.basename(projectRoot),
    watchRoot,
    runCount: store.getRunList().length,
  });
});

app.get("/api/runs", (_req, res) => {
  res.json({
    items: store.getRunList(),
    updatedAt: new Date().toISOString(),
  });
});

app.get("/api/runs/:runId", (req, res) => {
  const run = store.getRunDetail(req.params.runId);

  if (!run) {
    res.status(404).json({ error: "Run not found." });
    return;
  }

  res.json(run);
});

app.get("/api/assets/:runId/:fileName", (req, res) => {
  const run = store.getRunInternal(req.params.runId);

  if (!run) {
    res.status(404).json({ error: "Run not found." });
    return;
  }

  const requestedName = req.params.fileName;
  const image = run.files.images.find((item) => item.name === requestedName);

  if (!image) {
    res.status(404).json({ error: "Asset not found." });
    return;
  }

  res.sendFile(image.absolutePath);
});

app.get("/api/events", (req, res) => {
  res.setHeader("Content-Type", "text/event-stream");
  res.setHeader("Cache-Control", "no-cache, no-transform");
  res.setHeader("Connection", "keep-alive");
  res.flushHeaders?.();

  const sendEvent = (payload) => {
    res.write(`data: ${JSON.stringify(payload)}\n\n`);
  };

  sendEvent({
    type: "connected",
    timestamp: new Date().toISOString(),
    runCount: store.getRunList().length,
  });

  const unsubscribe = store.subscribe((payload) => sendEvent(payload));
  const heartbeat = setInterval(() => {
    sendEvent({ type: "heartbeat", timestamp: new Date().toISOString() });
  }, 15000);

  req.on("close", () => {
    clearInterval(heartbeat);
    unsubscribe();
    res.end();
  });
});

app.post("/api/chat", async (req, res) => {
  const { runId, apiConfig, messages } = req.body ?? {};
  const normalizedMessages = normalizeMessages(messages);

  if (!apiConfig?.baseUrl || !apiConfig?.apiKey || !apiConfig?.model) {
    res.status(400).json({ error: "Missing apiConfig.baseUrl, apiConfig.apiKey or apiConfig.model." });
    return;
  }

  if (!normalizedMessages.length) {
    res.status(400).json({ error: "At least one user message is required." });
    return;
  }

  const run = runId ? store.getRunInternal(runId) : null;
  const systemPrompt = buildSystemPrompt(apiConfig.systemPrompt);
  const contextBlock = buildContextBlock(run);
  const providerMessages = await buildMessagesForApi({
    messages: normalizedMessages,
    run,
    includeImages: apiConfig.includeImages !== false,
    contextBlock,
    systemPrompt,
  });

  const response = await fetch(buildChatEndpoint(apiConfig.baseUrl), {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${apiConfig.apiKey}`,
    },
    body: JSON.stringify({
      model: apiConfig.model,
      temperature: 0.2,
      messages: providerMessages,
    }),
  }).catch((error) => {
    return {
      ok: false,
      status: 502,
      async text() {
        return error instanceof Error ? error.message : "Unknown network error";
      },
    };
  });

  if (!response.ok) {
    const errorText = await response.text();
    res.status(response.status || 502).json({
      error: "Chat provider request failed.",
      detail: errorText.slice(0, 2000),
    });
    return;
  }

  const payload = await response.json();
  const reply = extractAssistantText(payload);

  if (!reply) {
    res.status(502).json({
      error: "Unable to extract assistant reply from provider response.",
      detail: payload,
    });
    return;
  }

  res.json({
    reply,
    providerModel: payload?.model || apiConfig.model,
    usage: payload?.usage || null,
  });
});

app.listen(port, () => {
  console.log(`SimLog monitor is running on http://localhost:${port}`);
  console.log(`Watching simulation logs in ${watchRoot}`);
});

function buildChatEndpoint(baseUrl) {
  const trimmed = String(baseUrl || "").trim().replace(/\/+$/, "");
  return `${trimmed}/chat/completions`;
}
