# SimLog Monitor Demo

这是一个放在 `D:\GraduationProject\EV\Logs` 目录下的最小可行 demo，用来持续监听 MATLAB 自动驾驶仿真输出，并提供：

- 自动扫描/监听当前日志目录下的新运行结果
- 解析 `SimLog.json`，生成摘要、图表和基础诊断
- 展示同目录下的分析图片
- 在前端配置 OpenAI-compatible API，结合日志和图片进行多轮对话

## 启动

```bash
npm install
npm run dev
```

默认端口是 `3760`，启动后访问：

```text
http://localhost:3760
```

## 当前支持的输入

- `SimLog.json`
- `*.png / *.jpg / *.jpeg / *.webp`
- `*.mat` 目前只做文件存在性展示，未解析内容

## 目录约定

项目位于：

```text
D:\GraduationProject\EV\Logs\simlog-monitor-demo
```

服务默认监听项目的父目录，也就是：

```text
D:\GraduationProject\EV\Logs
```

并自动忽略当前项目自身目录与 `node_modules`。

## MVP 说明

当前版本是为了尽快验证整体闭环，重点是：

- 先跑通“日志监听 -> 数据解析 -> 前端看板 -> AI 对话”
- 用 `SimLog.json` 作为主要绘图数据源
- 用图片增强 AI 解释能力

后续建议迭代方向：

- 解析 `Data.mat`
- 对不同 MATLAB 导出版本做更细的 schema 兼容
- 增加流式聊天输出
- 增加图表筛选、自定义指标和多运行对比
