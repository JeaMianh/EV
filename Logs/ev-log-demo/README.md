# EV Log Copilot

这是一个放在 `D:\GraduationProject\EV\Logs` 下的最小可行 demo，用来监听 MATLAB 自动驾驶仿真日志目录，并提供：

- `FastAPI` 后端服务
- 自动扫描和实时监听新的仿真输出目录
- 读取 `SimLog.json` 并生成基础指标摘要
- 展示已有分析图片 `png`
- 在前端用折线图显示速度、距离和跟踪状态
- 支持图表放大查看
- 提供可持续对话的 AI 助手
- 提供独立的 `/settings` 设置页，用于前端配置 OpenAI-compatible `Base URL / API Key / Model`

## 当前支持的输入

- `SimLog.json`
- `*.png`
- `*.mat` 会被识别到，但当前 MVP 仅展示文件存在，不解析其内容

## 运行方式

```bash
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements.txt
python -m uvicorn server.app:app --reload --port 3000
```

启动后访问：

```text
http://localhost:3000/
```

设置页：

```text
http://localhost:3000/settings
```

## 使用说明

1. 左侧会自动列出 `Logs` 目录下可解析的仿真子目录。
2. 选择一个 run 后，中间区域会显示摘要指标、曲线和图片。
3. 图表卡片右上角提供“放大查看”，可以单独打开更大的曲线面板。
4. 进入 `/settings` 页面填写：
   - `Base URL`，例如 `https://api.openai.com/v1`
   - `API Key`
   - `Model`
5. 返回主工作台后，右侧聊天区可以持续提问。
6. 若在设置页勾选“附带当前分析图片”，后端会把当前选中的分析图转成 `data URL` 一并发给模型。
7. 前端的 `Chart.js` 已经放到 `public/vendor`，运行时不再依赖 Node 服务。

## 接下来适合继续迭代的方向

- 解析 `Data.mat` 并补充更完整的 MATLAB 信号
- 增加多 run 对比视图，用于批量调参
- 把启发式建议升级为可配置规则引擎
- 接入流式聊天输出
- 支持把 AI 总结导出为 markdown 或报告
