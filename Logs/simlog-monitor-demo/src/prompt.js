import fs from "node:fs/promises";

const DEFAULT_SYSTEM_PROMPT = [
  "你是自动驾驶仿真日志分析助手。",
  "你的任务是结合时间序列数据、自动生成的统计摘要以及图像，对当前这次 MATLAB 仿真做解释、排查和优化建议。",
  "回答时优先基于现有证据，不要捏造没有出现的数据。",
  "当结论只是推测时，请明确说明是推测，并指出下一步应该补哪些日志或实验。",
  "当用户询问图片时，优先解释图里可能反映的趋势、异常点、跟踪丢失、虚警或标定问题。",
].join("\n");

export function buildSystemPrompt(customPrompt) {
  const extra = String(customPrompt || "").trim();
  return extra ? `${DEFAULT_SYSTEM_PROMPT}\n\n附加偏好：\n${extra}` : DEFAULT_SYSTEM_PROMPT;
}

export function normalizeMessages(messages) {
  if (!Array.isArray(messages)) {
    return [];
  }

  return messages
    .map((message) => {
      if (message?.role !== "assistant" && message?.role !== "user") {
        return null;
      }

      const role = message.role;
      const content = typeof message?.content === "string" ? message.content.trim() : "";
      return { role, content };
    })
    .filter((message) => message?.content);
}

export function buildContextBlock(run) {
  if (!run) {
    return [
      "当前没有选中具体仿真运行。",
      "如果用户问题依赖某次仿真，请提醒他先选择一条运行记录。",
    ].join("\n");
  }

  const lines = [
    `当前运行目录: ${run.id}`,
    `状态: ${run.status}`,
    `最近更新时间: ${run.updatedAt}`,
    `可用文件: SimLog=${run.available.hasSimLog}, Images=${run.available.hasImages}, MAT=${run.available.hasMat}`,
    "",
    "摘要指标:",
    JSON.stringify(run.summary, null, 2),
    "",
    "SimInfo:",
    JSON.stringify(run.simInfo || {}, null, 2),
    "",
    "自动洞察:",
    JSON.stringify(run.insights || [], null, 2),
    "",
    "优化建议:",
    JSON.stringify(run.suggestions || [], null, 2),
    "",
    "可视化图表:",
    JSON.stringify(
      (run.charts || []).map((chart) => ({
        id: chart.id,
        title: chart.title,
        description: chart.description,
        series: chart.series.map((item) => item.key),
      })),
      null,
      2,
    ),
  ];

  return lines.join("\n");
}

export async function buildMessagesForApi({
  messages,
  run,
  includeImages,
  contextBlock,
  systemPrompt,
}) {
  const imageParts = includeImages ? await loadRunImages(run) : [];
  const lastUserIndex = findLastUserIndex(messages);
  const preparedMessages = [
    { role: "system", content: systemPrompt },
    { role: "system", content: contextBlock },
  ];

  for (let index = 0; index < messages.length; index += 1) {
    const message = messages[index];
    let content = message.content;

    if (message.role === "user" && index === lastUserIndex && imageParts.length) {
      const parts = [
        { type: "text", text: message.content },
        {
          type: "text",
          text: `以下附带了当前运行的 ${imageParts.length} 张分析图，请结合图片和日志上下文回答。`,
        },
        ...imageParts,
      ];
      content = parts;
    }

    preparedMessages.push({
      role: message.role,
      content,
    });
  }

  return preparedMessages;
}

export function extractAssistantText(payload) {
  const choiceContent = payload?.choices?.[0]?.message?.content;

  if (typeof choiceContent === "string" && choiceContent.trim()) {
    return choiceContent.trim();
  }

  if (Array.isArray(choiceContent)) {
    const joined = choiceContent
      .map((part) => {
        if (typeof part === "string") {
          return part;
        }
        if (part?.type === "text" && typeof part?.text === "string") {
          return part.text;
        }
        return "";
      })
      .join("\n")
      .trim();

    if (joined) {
      return joined;
    }
  }

  if (typeof payload?.output_text === "string" && payload.output_text.trim()) {
    return payload.output_text.trim();
  }

  return "";
}

async function loadRunImages(run) {
  if (!run?.files?.images?.length) {
    return [];
  }

  const imageParts = [];
  let totalBytes = 0;

  for (const image of run.files.images.slice(0, 2)) {
    if (image.sizeBytes > 3 * 1024 * 1024) {
      continue;
    }

    if (totalBytes + image.sizeBytes > 4 * 1024 * 1024) {
      break;
    }

    const buffer = await fs.readFile(image.absolutePath);
    totalBytes += buffer.byteLength;
    imageParts.push({
      type: "image_url",
      image_url: {
        url: `data:${image.mimeType};base64,${buffer.toString("base64")}`,
      },
    });
  }

  return imageParts;
}

function findLastUserIndex(messages) {
  for (let index = messages.length - 1; index >= 0; index -= 1) {
    if (messages[index]?.role === "user") {
      return index;
    }
  }

  return -1;
}
