import json
from pathlib import Path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# =========================
# 用户配置区
# =========================

D0 = 5.0
TH = 1.5
V_SET = 5.0

EGO_LENGTH = 4.7
LEAD_LENGTH = 4.7

# 按你的视频/仿真路线微调这些时间窗口
SCENARIOS = {
    "uniform_following": {
        "time": (8.0, 38.0),
        "fig": "fig_01_uniform_following.png",
        "title": "Uniform Following Scenario",
        "kind": "longitudinal",
    },
    "s_curve_tracking": {
        "time": (112.0, 128.0),
        "fig": "fig_02_s_curve_tracking.png",
        "title": "S-Curve / Turning Tracking Scenario",
        "kind": "lateral",
    },
    "deceleration_stop": {
        "time": (38.0, 62.0),
        "fig": "fig_03_deceleration_stop.png",
        "title": "Lead Vehicle Deceleration-to-Stop Scenario",
        "kind": "longitudinal",
    },
    "low_speed_following": {
        "time": (62.0, 95.0),
        "fig": "fig_04_low_speed_following.png",
        "title": "Low-Speed Following and Restart Scenario",
        "kind": "low_speed",
    },
    "lane_change_braking": {
        "time": (128.0, 138.5),
        "fig": "fig_05_lane_change_braking.png",
        "title": "Lane-Change Near-Target Braking Scenario",
        "kind": "lane_change",
    },
}


# =========================
# 基础工具函数
# =========================

def get_signal(df, names, default=None):
    """按多个候选名读取信号。"""
    for name in names:
        if name in df.columns:
            return df[name].to_numpy(dtype=float)
    if default is None:
        return None
    return np.full(len(df), default, dtype=float)


def load_json_as_df(json_path):
    with open(json_path, "r", encoding="utf-8") as f:
        raw = json.load(f)

    n = len(raw["time"])
    usable = {}

    for k, v in raw.items():
        if isinstance(v, list) and len(v) == n:
            usable[k] = v

    df = pd.DataFrame(usable)
    return df


def add_basic_signals(df):
    """只补论文需要的基础派生信号。"""
    df = df.copy()

    t = df["time"].to_numpy(dtype=float)

    # 速度
    if "v_ego" not in df.columns:
        raise ValueError("缺少 v_ego，无法画纵向速度图。")

    # 前车速度：优先使用已有 v_lead，否则用 leadX/leadY 差分
    if "v_lead" not in df.columns:
        if {"leadX", "leadY"}.issubset(df.columns):
            lead_x = df["leadX"].to_numpy(dtype=float)
            lead_y = df["leadY"].to_numpy(dtype=float)
            lead_vx = np.gradient(lead_x, t)
            lead_vy = np.gradient(lead_y, t)
            df["v_lead"] = np.sqrt(lead_vx ** 2 + lead_vy ** 2)
        elif "v_rel_truth" in df.columns:
            df["v_lead"] = df["v_ego"] + df["v_rel_truth"]
        else:
            df["v_lead"] = np.nan

    # 实际欧氏车距：有 XY 就用 XY 算；否则用 d_rel_truth
    if {"egoX", "egoY", "leadX", "leadY"}.issubset(df.columns):
        dx = df["leadX"].to_numpy(dtype=float) - df["egoX"].to_numpy(dtype=float)
        dy = df["leadY"].to_numpy(dtype=float) - df["egoY"].to_numpy(dtype=float)
        center_dist = np.sqrt(dx ** 2 + dy ** 2)
        bumper_comp = 0.5 * (EGO_LENGTH + LEAD_LENGTH)
        df["realGap"] = np.maximum(center_dist - bumper_comp, 0.0)

        # 变道图用这个，不强行说它是严格车体坐标横向误差
        df["y_separation"] = df["leadY"].to_numpy(dtype=float) - df["egoY"].to_numpy(dtype=float)
    else:
        df["realGap"] = df.get("d_rel_truth", np.nan)
        df["y_separation"] = np.nan

    # 纵向距离
    if "d_rel_truth" in df.columns:
        df["d_long"] = df["d_rel_truth"]
    else:
        df["d_long"] = df["realGap"]

    # 期望距离
    df["d_ref"] = D0 + TH * df["v_ego"]

    # 距离误差
    df["gap_error"] = df["d_long"] - df["d_ref"]

    # 车头时距
    df["time_gap"] = df["d_long"] / np.maximum(df["v_ego"], 0.1)

    # 本车加速度
    df["a_ego"] = np.gradient(df["v_ego"].to_numpy(dtype=float), t)

    return df


def crop(df, t0, t1):
    return df[(df["time"] >= t0) & (df["time"] <= t1)].copy()


def rel_time(df):
    return df["time"].to_numpy(dtype=float) - df["time"].iloc[0]


def save_fig(fig, out_dir, name):
    out_dir.mkdir(parents=True, exist_ok=True)
    path = out_dir / name
    fig.tight_layout()
    fig.savefig(path, dpi=300)
    plt.close(fig)
    return path


# =========================
# 图 1/3：纵向工况通用图
# =========================

def plot_longitudinal_scenario(df, out_dir, fig_name, title):
    """
    适用于：
    - 匀速跟车
    - 前车减速停车

    三个子图：
    1. 本车/前车速度
    2. 相对距离/期望距离
    3. 加速/制动命令
    """
    x = rel_time(df)

    acc = get_signal(df, ["acc_post", "AccelCmd_post", "ThrottleCmd", "AccelOut"], default=0.0)
    brk = get_signal(df, ["brk_post", "BrakeCmd_post", "BrakeCmd", "BrakeOut"], default=0.0)

    fig, axes = plt.subplots(3, 1, figsize=(8.5, 7.0), sharex=True)

    axes[0].plot(x, df["v_ego"], label="Ego speed")
    axes[0].plot(x, df["v_lead"], label="Lead speed")
    axes[0].axhline(V_SET, linestyle="--", linewidth=1, label="Set speed")
    axes[0].set_ylabel("Speed / (m/s)")
    axes[0].grid(True)
    axes[0].legend(loc="best")

    axes[1].plot(x, df["d_long"], label="Longitudinal gap")
    axes[1].plot(x, df["d_ref"], linestyle="--", label="Desired gap")
    axes[1].set_ylabel("Distance / m")
    axes[1].grid(True)
    axes[1].legend(loc="best")

    axes[2].plot(x, acc, label="Accel command")
    axes[2].plot(x, brk, label="Brake command")
    axes[2].set_xlabel("Time / s")
    axes[2].set_ylabel("Command")
    axes[2].grid(True)
    axes[2].legend(loc="best")

    fig.suptitle(title)

    return save_fig(fig, out_dir, fig_name)


# =========================
# 图 2：S 弯 / 转弯横向工况
# =========================

def plot_lateral_scenario(df, out_dir, fig_name, title):
    """
    两个子图：
    1. 车辆轨迹
    2. 横向误差或转向命令

    如果没有 e_y，就不硬画不存在的误差信号。
    """
    x = rel_time(df)

    fig, axes = plt.subplots(2, 1, figsize=(8.5, 7.0))

    # 轨迹图
    if {"egoX", "egoY"}.issubset(df.columns):
        axes[0].plot(df["egoX"], df["egoY"], label="Ego trajectory")

        if {"leadX", "leadY"}.issubset(df.columns):
            axes[0].plot(df["leadX"], df["leadY"], linestyle="--", label="Lead trajectory")

        axes[0].set_xlabel("X / m")
        axes[0].set_ylabel("Y / m")
        axes[0].axis("equal")
        axes[0].grid(True)
        axes[0].legend(loc="best")
    else:
        axes[0].text(0.5, 0.5, "No trajectory signals", ha="center", va="center")
        axes[0].axis("off")

    # 横向误差优先，其次转向命令
    lateral_error = get_signal(
        df,
        ["e_y", "lateralError", "lat_error", "lateral_error"],
        default=None,
    )
    steer = get_signal(
        df,
        ["steerCmd", "SteerCmd", "SteeringCmd", "SteerOut"],
        default=None,
    )

    if lateral_error is not None:
        axes[1].plot(x, lateral_error, label="Lateral error")
        axes[1].set_ylabel("Lateral error / m")
    elif steer is not None:
        axes[1].plot(x, steer, label="Steering command")
        axes[1].set_ylabel("Steering command")
    else:
        # 没有横向误差/转角，就画 Y 轨迹变化，至少能说明 S 弯/转弯过程
        if "egoY" in df.columns:
            axes[1].plot(x, df["egoY"], label="Ego Y position")
            axes[1].set_ylabel("Y / m")
        else:
            axes[1].text(0.5, 0.5, "No lateral signal", ha="center", va="center")
            axes[1].axis("off")

    axes[1].set_xlabel("Time / s")
    axes[1].grid(True)
    axes[1].legend(loc="best")

    fig.suptitle(title)

    return save_fig(fig, out_dir, fig_name)


# =========================
# 图 4：低速跟车再起步
# =========================

def plot_low_speed_scenario(df, out_dir, fig_name, title):
    """
    三个子图：
    1. 低速速度响应
    2. 距离
    3. Hold 与制动命令

    这个图专门支撑“Stop&Hold 与释放逻辑”。
    """
    x = rel_time(df)

    brk = get_signal(df, ["brk_post", "BrakeCmd_post", "BrakeCmd", "BrakeOut"], default=0.0)
    hold = get_signal(df, ["holdActive"], default=0.0)

    fig, axes = plt.subplots(3, 1, figsize=(8.5, 7.0), sharex=True)

    axes[0].plot(x, df["v_ego"], label="Ego speed")
    axes[0].plot(x, df["v_lead"], label="Lead speed")
    axes[0].set_ylabel("Speed / (m/s)")
    axes[0].grid(True)
    axes[0].legend(loc="best")

    axes[1].plot(x, df["d_long"], label="Longitudinal gap")
    axes[1].plot(x, df["d_ref"], linestyle="--", label="Desired gap")
    axes[1].set_ylabel("Distance / m")
    axes[1].grid(True)
    axes[1].legend(loc="best")

    axes[2].plot(x, brk, label="Brake command")
    axes[2].plot(x, hold, linestyle="--", label="Hold active")
    axes[2].set_xlabel("Time / s")
    axes[2].set_ylabel("Command / State")
    axes[2].grid(True)
    axes[2].legend(loc="best")

    fig.suptitle(title)

    return save_fig(fig, out_dir, fig_name)


# =========================
# 图 5：变道后近距离目标接入制动
# =========================

def plot_lane_change_braking(df, out_dir, fig_name, title):
    """
    三个子图：
    1. XY 轨迹，说明变道
    2. 横向间隔，说明目标接入
    3. 车距 + 制动命令，说明制动响应

    这个图不画 AEB，不画 TTC，避免日志状态不好时干扰论文叙述。
    """
    x = rel_time(df)

    brk = get_signal(df, ["brk_post", "BrakeCmd_post", "BrakeCmd", "BrakeOut"], default=0.0)

    fig, axes = plt.subplots(3, 1, figsize=(8.5, 8.0))

    if {"egoX", "egoY"}.issubset(df.columns):
        axes[0].plot(df["egoX"], df["egoY"], label="Ego trajectory")
        if {"leadX", "leadY"}.issubset(df.columns):
            axes[0].plot(df["leadX"], df["leadY"], linestyle="--", label="Lead trajectory")
        axes[0].set_xlabel("X / m")
        axes[0].set_ylabel("Y / m")
        axes[0].axis("equal")
        axes[0].grid(True)
        axes[0].legend(loc="best")
    else:
        axes[0].text(0.5, 0.5, "No trajectory signals", ha="center", va="center")
        axes[0].axis("off")

    axes[1].plot(x, df["y_separation"], label="Lateral separation")
    axes[1].axhline(2.5, linestyle="--", linewidth=1, label="Lane threshold")
    axes[1].axhline(-2.5, linestyle="--", linewidth=1)
    axes[1].set_xlabel("Time / s")
    axes[1].set_ylabel("Lateral separation / m")
    axes[1].grid(True)
    axes[1].legend(loc="best")

    ax_dist = axes[2]
    ax_dist.plot(x, df["realGap"], label="Real gap")
    ax_dist.plot(x, df["d_ref"], linestyle="--", label="Desired gap")
    ax_dist.set_xlabel("Time / s")
    ax_dist.set_ylabel("Distance / m")
    ax_dist.grid(True)

    ax_brk = ax_dist.twinx()
    ax_brk.plot(x, brk, linestyle=":", label="Brake command")
    ax_brk.set_ylabel("Brake command")

    lines1, labels1 = ax_dist.get_legend_handles_labels()
    lines2, labels2 = ax_brk.get_legend_handles_labels()
    ax_dist.legend(lines1 + lines2, labels1 + labels2, loc="best")

    fig.suptitle(title)

    return save_fig(fig, out_dir, fig_name)


# =========================
# 统计表
# =========================

def summarize_scenario(df, name):
    acc = get_signal(df, ["acc_post", "AccelCmd_post", "ThrottleCmd", "AccelOut"], default=0.0)
    brk = get_signal(df, ["brk_post", "BrakeCmd_post", "BrakeCmd", "BrakeOut"], default=0.0)

    speed_error = df["v_ego"].to_numpy(dtype=float) - df["v_lead"].to_numpy(dtype=float)

    # 横向误差：有就统计，没有就空
    lat_err = get_signal(df, ["e_y", "lateralError", "lat_error", "lateral_error"], default=None)

    row = {
        "scenario": name,
        "duration_s": float(df["time"].iloc[-1] - df["time"].iloc[0]),
        "max_speed_error_mps": float(np.nanmax(np.abs(speed_error))),
        "min_gap_m": float(np.nanmin(df["d_long"])),
        "min_real_gap_m": float(np.nanmin(df["realGap"])),
        "min_time_gap_s": float(np.nanmin(df["time_gap"])),
        "max_brake_cmd": float(np.nanmax(brk)),
        "max_accel_cmd": float(np.nanmax(acc)),
        "max_deceleration_mps2": float(max(0.0, -np.nanmin(df["a_ego"]))),
    }

    if lat_err is not None:
        row["max_lateral_error_m"] = float(np.nanmax(np.abs(lat_err)))
        row["mean_lateral_error_m"] = float(np.nanmean(np.abs(lat_err)))
    else:
        row["max_lateral_error_m"] = np.nan
        row["mean_lateral_error_m"] = np.nan

    return row


# =========================
# 主函数
# =========================

def plot_paper_scenarios(json_path, out_dir="paper_figures_selected"):
    """
    按论文第5章工况出图。

    输出：
    - 5 张工况图
    - paper_metrics_summary.csv
    """
    out_dir = Path(out_dir)

    df = load_json_as_df(json_path)
    df = add_basic_signals(df)

    saved = []
    metrics = []

    for name, cfg in SCENARIOS.items():
        t0, t1 = cfg["time"]
        sub = crop(df, t0, t1)

        if len(sub) < 3:
            print(f"[Skip] {name}: no enough data in time window {t0}-{t1}")
            continue

        kind = cfg["kind"]

        if kind == "longitudinal":
            path = plot_longitudinal_scenario(
                sub, out_dir, cfg["fig"], cfg["title"]
            )
        elif kind == "lateral":
            path = plot_lateral_scenario(
                sub, out_dir, cfg["fig"], cfg["title"]
            )
        elif kind == "low_speed":
            path = plot_low_speed_scenario(
                sub, out_dir, cfg["fig"], cfg["title"]
            )
        elif kind == "lane_change":
            path = plot_lane_change_braking(
                sub, out_dir, cfg["fig"], cfg["title"]
            )
        else:
            continue

        saved.append(path)
        metrics.append(summarize_scenario(sub, name))

    metrics_df = pd.DataFrame(metrics)
    metrics_path = out_dir / "paper_metrics_summary.csv"
    metrics_df.to_csv(metrics_path, index=False, encoding="utf-8-sig")
    saved.append(metrics_path)

    return metrics_df, saved


if __name__ == "__main__":
    # 改成你的 JSON 文件路径
    json_path = r"D:\GraduationProject\EV\Logs\20260424_105443\SimLog.json"

    metrics_df, saved_files = plot_paper_scenarios(
        json_path=json_path,
        out_dir="paper_figures_selected",
    )

    print(metrics_df)
    print("\nSaved files:")
    for p in saved_files:
        print(p)