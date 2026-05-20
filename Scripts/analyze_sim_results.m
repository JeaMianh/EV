% function analyze_sim_results(out)
%     % ANALYZE_SIM_RESULTS 自动分析驾驶仿真数据
%     % 功能：对齐时间轴，计算误差，识别幽灵物体，导出可视化图表与 JSON
% 
%     %% 1. 初始化设置
%     % 配置保存路径 (按时间戳)
%     logRoot = 'D:\GraduationProject\EV\Logs';
%     timestamp = string(datetime('now', 'Format', 'yyyyMMdd_HHmmss'));
%     saveDir = fullfile(logRoot, timestamp);
% 
%     % 检查数据源
%     if ~isprop(out, 'LogData')
%         error('错误: 输出数据中未找到 LogData，请检查 Simulink 的 Signal Logging 设置。');
%     end
%     logs = out.LogData;
% 
%     % 检查必要信号
%     if ~isfield(logs, 'EgoState') || ~isfield(logs.EgoState, 'X')
%         error('错误: 缺少本车位置数据 (EgoState.X)，无法分析。');
%     end
% 
%     % 创建目录
%     if ~exist(saveDir, 'dir'), mkdir(saveDir); end
%     fprintf('------------------------------------------------\n');
%     fprintf('>>> 开始分析仿真数据: %s\n', timestamp);
% 
%     %% 2. 数据提取与对齐
%     % 建立主时间轴 (以本车位置更新率为准)
%     masterTime = logs.EgoState.X.Time;
% 
%     % 定义对齐工具函数 (线性插值 & 最近邻插值)
%     align_lin = @(ts) safe_resample(ts, masterTime, 'linear');
%     align_id  = @(ts) safe_resample(ts, masterTime, 'nearest'); % ID不能插值，只能取最近
% 
%     % --- A. 本车数据 (Ego) ---
%     Ego.X = logs.EgoState.X.Data;
%     Ego.Y = logs.EgoState.Y.Data;
%     % 计算合速度 (XY矢量合成)
%     if isfield(logs.EgoState, 'Xdot')
%         Ego.Vx = logs.EgoState.Xdot.Data;
%         Ego.Vy = logs.EgoState.Ydot.Data;
%     else
%         % 备用：差分计算
%         Ego.Vx = [0; diff(Ego.X)./diff(masterTime)];
%         Ego.Vy = [0; diff(Ego.Y)./diff(masterTime)];
%     end
%     Ego.AbsSpeed = sqrt(Ego.Vx.^2 + Ego.Vy.^2);
% 
%     % --- B. 真值数据 (Ground Truth) ---
%     Target.Valid = false;
%     Target.Dist = nan(size(masterTime));
%     Target.RelVel = nan(size(masterTime));
%     Target.AbsSpeed = nan(size(masterTime));
% 
%     if isfield(logs, 'TargetState') && isfield(logs.TargetState, 'Location')
%         try
%             % 对齐真值坐标
%             tgt_loc = align_lin(logs.TargetState.Location);
%             Target.X = tgt_loc(:,1);
%             Target.Y = tgt_loc(:,2);
% 
%             % 计算真值距离
%             Target.Dist = sqrt((Target.X - Ego.X).^2 + (Target.Y - Ego.Y).^2);
% 
%             % 计算真值速度
%             tgt_vx = gradient(Target.X) ./ gradient(masterTime);
%             tgt_vy = gradient(Target.Y) ./ gradient(masterTime);
%             Target.AbsSpeed = sqrt(tgt_vx.^2 + tgt_vy.^2);
% 
%             % 计算真值相对速度
%             Target.RelVel = Target.AbsSpeed - Ego.AbsSpeed;
%             Target.Valid = true;
%         catch ME
%             warning('真值解析异常: %s', ME.message);
%         end
%     end
% 
%     % --- C. 算法输出 (Algorithm/Selection) ---
%     % 这是你的 MATLAB Function 选出来的“最终目标”
%     Algo.Dist = align_lin(logs.FunctionOutput.rel_dist);
%     Algo.Vel  = align_lin(logs.FunctionOutput.rel_vel);
% 
%     % 如果你的总线里加了 target_id，在这里提取
%     if isfield(logs.FunctionOutput, 'target_id')
%         Algo.ID = align_id(logs.FunctionOutput.target_id);
%     else
%         Algo.ID = zeros(size(masterTime)); % 默认占位
%     end
% 
%     % --- D. 追踪器原始数据 (Tracker Raw) ---
%     % 用于分析“看见了但没选”或者“幽灵物体”
%     % 假设 Log 中有 TrackerOutput.Tracks
%     num_max_tracks = 20;
%     Trk.AllDist = nan(length(masterTime), num_max_tracks);
%     Trk.AllIDs  = nan(length(masterTime), num_max_tracks);
% 
%     if isfield(logs, 'TrackerOutput')
%         rawTracks = logs.TrackerOutput.Tracks;
%         for i = 1:num_max_tracks
%             try
%                 % 提取第 i 条航迹的时间序列
%                 ts_state = rawTracks(i).State; 
%                 ts_id    = rawTracks(i).TrackID;
% 
%                 if isa(ts_state, 'timeseries') && length(ts_state.Time) > 1
%                     % 提取相对距离 (假设 State(1) 是 X)
%                     Trk.AllDist(:, i) = resample(timeseries(ts_state.Data(:,1), ts_state.Time), masterTime).Data;
%                     Trk.AllIDs(:, i)  = resample(timeseries(double(ts_id.Data), ts_id.Time), masterTime, 'nearest').Data;
%                 end
%             catch
%                 % 忽略空 track
%             end
%         end
%     end
% 
%     %% 3. 数据可视化
%     fprintf('>>> 生成分析图表...\n');
%     fig = figure('Name', 'SimAnalysis', 'Color', 'w', 'Position', [100, 100, 1400, 900]);
%     t = tiledlayout(2, 2, 'TileSpacing', 'compact');
%     title(t, ['仿真分析报告 - ' char(timestamp)], 'Interpreter', 'none');
% 
%     % 图1: 距离追踪与杂波分析 (Range Analysis)
%     nexttile; hold on; grid on; box on;
%     % 1.1 画出所有 Tracker 看到的物体 (灰色背景线) -> 用于发现幽灵
%     plot(masterTime, Trk.AllDist, 'Color', [0.85 0.85 0.85], 'LineWidth', 0.5, 'HandleVisibility', 'off');
%     % 1.2 画出真值 (绿色粗线)
%     plot(masterTime, Target.Dist, 'g', 'LineWidth', 2, 'DisplayName', 'Ground Truth');
%     % 1.3 画出算法最终锁定的目标 (红色虚线)
%     plot(masterTime, Algo.Dist, 'r--', 'LineWidth', 1.5, 'DisplayName', 'Selected Target');
%     % 1.4 标记 7.5m 警戒线
%     yline(7.5, 'b:', 'Alpha', 0.5, 'DisplayName', 'Ghost Line (7.5m)');
% 
%     ylabel('Distance (m)'); ylim([0, 150]);
%     title('1. 相对距离追踪 (含背景杂波)');
%     legend('Location', 'best');
% 
%     % 图2: 速度一致性校验 (Doppler Check)
%     nexttile; hold on; grid on; box on;
%     plot(masterTime, Target.RelVel, 'g', 'LineWidth', 2, 'DisplayName', 'GT RelVel');
%     plot(masterTime, Algo.Vel, 'r--', 'LineWidth', 1.5, 'DisplayName', 'Radar RelVel');
%     yline(0, 'k-', 'Alpha', 0.3);
%     ylabel('Rel Speed (m/s)');
%     title('2. 相对速度 (判断静止/运动)');
%     legend('Location', 'best');
% 
%     % 图3: 目标 ID 稳定性 (ID Stability)
%     nexttile; hold on; grid on; box on;
%     % 过滤掉 ID=0 (未选中) 的点
%     clean_ids = Algo.ID; clean_ids(clean_ids==0) = nan;
%     plot(masterTime, clean_ids, 'b.', 'MarkerSize', 10);
%     ylabel('Target ID');
%     title('3. 锁定目标的 ID 切换情况');
%     subtitle('频繁跳变意味着 Tracker 不稳定');
% 
%     % 图4: 探测误差分析 (Error Analysis)
%     nexttile; hold on; grid on; box on;
%     err = abs(Algo.Dist - Target.Dist);
%     % 只有当雷达看到东西(dist<190)时才计算误差
%     err(Algo.Dist > 190) = nan; 
%     area(masterTime, err, 'FaceColor', 'r', 'FaceAlpha', 0.3, 'EdgeColor', 'none');
%     yline(1.0, 'k--', 'Label', 'Tolerance 1m');
%     ylabel('Abs Error (m)'); ylim([0, 5]);
%     title('4. 测距误差 (GT vs Radar)');
% 
%     % 保存图片
%     saveas(fig, fullfile(saveDir, 'Analysis_Report.png'));
% 
%     %% 4. 导出关键数据到 JSON
%     fprintf('>>> 导出 JSON 数据...\n');
% 
%     % 降采样 (每 0.1s 存一个点，减小文件体积)
%     step = max(1, floor(0.1 / mean(diff(masterTime))));
%     idx = 1:step:length(masterTime);
% 
%     JData.Time = masterTime(idx);
%     JData.SimInfo.RCS_Setting = 10; % 记录当前参数，方便对比
%     JData.SimInfo.Pitch_Setting = 1; 
% 
%     % 核心数据
%     JData.EgoSpd = Ego.AbsSpeed(idx);
%     JData.TgtSpd = Target.AbsSpeed(idx);
%     JData.GT_Dist = Target.Dist(idx);
%     JData.Radar_Dist = Algo.Dist(idx);
%     JData.Radar_RelVel = Algo.Vel(idx);
% 
%     % --- 关键新增数据 ---
% 
%     % 1. 目标 ID (判断是否切车)
%     JData.Target_ID = Algo.ID(idx);
% 
%     % 2. 幽灵物体标志位 (Ghost Flag)
%     % 逻辑：如果锁定距离在 5m-10m 之间，但真值距离大于 15m，标记为幽灵
%     is_ghost = (Algo.Dist(idx) > 5.0 & Algo.Dist(idx) < 10.0) & (Target.Dist(idx) > 15.0);
%     JData.Flag_Ghost = double(is_ghost);
% 
%     % 3. 丢失目标标志位 (Loss Flag)
%     % 逻辑：真值在 150m 以内，但雷达读数 > 190m (未检测到)
%     is_loss = (Target.Dist(idx) < 150.0) & (Algo.Dist(idx) > 190.0);
%     JData.Flag_Loss = double(is_loss);
% 
%     % 保存
%     jsonStr = jsonencode(JData, 'PrettyPrint', true);
%     fid = fopen(fullfile(saveDir, 'SimLog.json'), 'w');
%     if fid > 0
%         fwrite(fid, jsonStr, 'char');
%         fclose(fid);
%         fprintf('    JSON 已保存: %s\n', fullfile(saveDir, 'SimLog.json'));
%     end
% 
%     fprintf('>>> ✅ 分析完成！\n');
% end
% 
% % % --- 辅助函数：安全重采样 ---
% % function out_data = safe_resample(timeseries_obj, target_time, method)
% %     if isempty(timeseries_obj) || length(timeseries_obj.Time) < 2
% %         out_data = nan(size(target_time));
% %         return;
% %     end
% %     try
% %         ts_res = resample(timeseries_obj, target_time, method);
% %         out_data = ts_res.Data;
% %     catch
% %         out_data = nan(size(target_time));
% %     end
% % end
% 
% % --- 辅助函数：安全重采样 (修复版 v2) ---
% function out_data = safe_resample(ts_obj, target_time, method)
%     % 1. 基础检查
%     if isempty(ts_obj) || length(ts_obj.Time) < 2
%         out_data = nan(size(target_time));
%         return;
%     end
% 
%     try
%         % 2. 提取原始数据
%         t_raw = ts_obj.Time;
%         d_raw = ts_obj.Data;
% 
%         % 3. 去重 (interp1 不允许时间轴有重复点)
%         [t_unique, idx] = unique(t_raw);
%         d_unique = d_raw(idx, :);
% 
%         % 4. 使用 interp1 进行插值
%         % 关键点：最后一个参数 NaN 表示“超出范围的部分填空值”，不要报错
%         if strcmp(method, 'zoh') || strcmp(method, 'nearest')
%             % 处理 ID 类离散数据
%             out_data = interp1(t_unique, d_unique, target_time, 'nearest', 'extrap');
%         else
%             % 处理连续数值 (线性插值 + 边界填 NaN)
%             out_data = interp1(t_unique, d_unique, target_time, 'linear', NaN);
%         end
%     catch ME
%         % 如果还出错，打印日志并填空
%         fprintf('    [Warning] 重采样失败: %s\n', ME.message);
%         out_data = nan(size(target_time));
%     end
% end



function analyze_sim_results(out)
% ANALYZE_SIM_RESULTS
% 基于当前 LogModule 信号自动分析并导出图表 + JSON + MAT
%
% 兼容两套命名：
% 旧：d_rel_truth / v_rel_truth / d_rel_radar / v_rel_radar / v_ego ...
% 新：d_gap_raw   / v_rel_raw   / rel_dist    / rel_vel    / v ...
%
% 轨迹兼容：
% 新：egoX/egoY/leadX/leadY
% 旧：egoT/leadT (Nx2 或 Nx3)
%
% 用法：
%   analyze_sim_results(out)            % out 为 SimulationOutput
%   analyze_sim_results(LogData)        % 直接传 Dataset/struct 也可

%% 1) 保存目录
logRoot = 'D:\GraduationProject\EV\Logs';
timestamp = string(datetime('now','Format','yyyyMMdd_HHmmss'));
saveDir = fullfile(logRoot, timestamp);
if ~exist(saveDir,'dir'); mkdir(saveDir); end

fprintf('\n------------------------------------------------\n');
fprintf('>>> 开始分析: %s\n', timestamp);

%% 2) 日志容器
logs = get_log_container(out);

% 主时间轴：按别名优先顺序找
masterTime = get_master_time(logs, { ...
    {'v_ego','v'}, ...
    {'d_rel_truth','d_gap_raw'}, ...
    {'d_rel_radar','rel_dist'}, ...
    {'a_cmd','acceleration'} ...
    });

if isempty(masterTime)
    warning('未找到可用主时间轴，跳过本次分析。');
    return;
end
masterTime = masterTime(:);

%% 3) 读取并对齐信号（别名兼容）
S.time = masterTime;

S.d_rel_truth  = get_sig(logs, {'d_rel_truth','d_gap_raw'}, masterTime, 'linear');
S.v_rel_truth  = get_sig(logs, {'v_rel_truth','v_rel_raw'}, masterTime, 'linear');
S.d_rel_radar  = get_sig(logs, {'d_rel_radar','rel_dist','d_rel'}, masterTime, 'linear');
S.v_rel_radar  = get_sig(logs, {'v_rel_radar','rel_vel','v_rel'}, masterTime, 'linear');
S.v_ego        = get_sig(logs, {'v_ego','v'}, masterTime, 'linear');

S.a_cmd        = get_sig(logs, {'a_cmd','acceleration'}, masterTime, 'linear');
S.acc_pre      = get_sig(logs, {'AccelCmd_pre','AccelCmd'}, masterTime, 'linear');
S.brk_pre      = get_sig(logs, {'BrakeCmd_pre','BrakeCmd'}, masterTime, 'linear');
S.acc_post     = get_sig(logs, {'AccelCmd_post','AccelOut'}, masterTime, 'linear');
S.brk_post     = get_sig(logs, {'BrakeCmd_post','BrakeOut'}, masterTime, 'linear');

S.steer_pre    = get_sig(logs, {'SteerCmd_pre','steer_norm'}, masterTime, 'linear');
S.steer_post   = get_sig(logs, {'SteerCmd_post','steer_out'}, masterTime, 'linear');

S.holdActive   = get_sig(logs, {'holdActive'}, masterTime, 'nearest');
S.aebModeOut   = get_sig(logs, {'aebModeOut'}, masterTime, 'nearest');
S.TTCOut       = get_sig(logs, {'TTCOut'}, masterTime, 'linear');
S.dUseOut      = get_sig(logs, {'dUseOut'}, masterTime, 'linear');
S.vrelUseOut   = get_sig(logs, {'vrelUseOut'}, masterTime, 'linear');
S.cipv_valid   = get_sig(logs, {'cipv_valid'}, masterTime, 'nearest');
S.cipvEffOut   = get_sig(logs, {'cipvEffOut'}, masterTime, 'nearest');

% 轨迹（新命名 + 旧命名兼容）
S.egoX  = get_sig(logs, {'egoX','ego_x','EgoX'}, masterTime, 'linear');
S.egoY  = get_sig(logs, {'egoY','ego_y','EgoY'}, masterTime, 'linear');
S.leadX = get_sig(logs, {'leadX','lead_x','LeadX'}, masterTime, 'linear');
S.leadY = get_sig(logs, {'leadY','lead_y','LeadY'}, masterTime, 'linear');

% 旧接口（可选）
S.egoT  = get_sig(logs, {'egoT'}, masterTime, 'linear');
S.leadT = get_sig(logs, {'leadT'}, masterTime, 'linear');

S = normalize_shapes(S);

% 统一轨迹：优先新命名，若无则回退旧命名
[S.egoXY, hasEgoXY]   = build_xy_track(S.egoX, S.egoY, S.egoT);
[S.leadXY, hasLeadXY] = build_xy_track(S.leadX, S.leadY, S.leadT);

%% 4) 派生统计
validMask = isfinite(S.d_rel_truth) & isfinite(S.d_rel_radar) & (S.d_rel_truth < 190);

if any(validMask)
    err_dist = S.d_rel_radar - S.d_rel_truth;
    MAE  = mean(abs(err_dist(validMask)), 'omitnan');
    RMSE = sqrt(mean(err_dist(validMask).^2, 'omitnan'));
else
    err_dist = nan(size(S.time));
    MAE = NaN; RMSE = NaN;
end

dmin_truth = min(S.d_rel_truth, [], 'omitnan');
dmin_radar = min(S.d_rel_radar, [], 'omitnan');

tg = S.d_rel_truth ./ max(S.v_ego, 0.1);
tgMin = min(tg, [], 'omitnan');

aebMode = round(fill_nan_with(S.aebModeOut,0));
aebTrig = sum(diff(aebMode > 0) == 1);
fbTrig  = sum(diff(aebMode == 3) == 1);
holdRatio = mean(S.holdActive > 0.5, 'omitnan');

delta_brk = S.brk_post - S.brk_pre;
delta_acc = S.acc_post - S.acc_pre;
brkOverrideRatio = mean(delta_brk > 1e-3, 'omitnan');
accCutRatio      = mean(delta_acc < -1e-3, 'omitnan');

if hasEgoXY && hasLeadXY
    d_xy = hypot(S.leadXY(:,1)-S.egoXY(:,1), S.leadXY(:,2)-S.egoXY(:,2));
else
    d_xy = nan(size(S.time));
end

% %% 5) 画图
% fprintf('>>> 生成图表...\n');
% fig = figure('Name','SimAnalysis','Color','w','Position',[100 80 1500 980]);
% tl = tiledlayout(3,2,'TileSpacing','compact');
% title(tl, ['仿真分析报告 - ' char(timestamp)], 'Interpreter','none');
% 
% % 图1 距离
% nexttile; hold on; grid on; box on;
% plot(S.time, S.d_rel_truth, 'g','LineWidth',2,'DisplayName','Truth d_{rel}');
% plot(S.time, S.d_rel_radar, 'r--','LineWidth',1.5,'DisplayName','Radar d_{rel}');
% plot(S.time, S.dUseOut, 'b-.','LineWidth',1.2,'DisplayName','Supervisor dUse');
% ylabel('Distance (m)'); title('1) 相对距离');
% legend('Location','best');
% 
% % 图2 相对速度
% nexttile; hold on; grid on; box on;
% plot(S.time, S.v_rel_truth, 'g','LineWidth',2,'DisplayName','Truth v_{rel}');
% plot(S.time, S.v_rel_radar, 'r--','LineWidth',1.5,'DisplayName','Radar v_{rel}');
% plot(S.time, S.vrelUseOut, 'b-.','LineWidth',1.2,'DisplayName','Supervisor vrelUse');
% yline(0,'k:');
% ylabel('RelVel (m/s)'); title('2) 相对速度');
% legend('Location','best');
% 
% % 图3 纵向命令 pre/post
% nexttile; hold on; grid on; box on;
% plot(S.time, S.acc_pre,  'Color',[0.3 0.8 0.3],'DisplayName','Accel pre');
% plot(S.time, S.brk_pre,  'Color',[0.9 0.5 0.2],'DisplayName','Brake pre');
% plot(S.time, S.acc_post, 'g','LineWidth',1.3,'DisplayName','Accel post');
% plot(S.time, S.brk_post, 'r','LineWidth',1.3,'DisplayName','Brake post');
% ylabel('Pedal Cmd (0~1)'); title('3) 监督前后命令');
% legend('Location','best');
% 
% % 图4 状态机
% nexttile; hold on; grid on; box on;
% plot(S.time, S.cipv_valid, 'k-','LineWidth',1.2,'DisplayName','cipv\_valid');
% plot(S.time, S.holdActive, 'b-','LineWidth',1.2,'DisplayName','holdActive');
% plot(S.time, S.aebModeOut, 'm-','LineWidth',1.2,'DisplayName','aebModeOut');
% ylabel('State'); title('4) 监督状态');
% legend('Location','best');
% 
% % 图5 TTC
% nexttile; hold on; grid on; box on;
% plot(S.time, S.TTCOut, 'c','LineWidth',1.3);
% yline(1.8,'k--','FCW');
% yline(1.1,'k--','PB1');
% yline(0.7,'k--','FB');
% finiteTTC = S.TTCOut(isfinite(S.TTCOut));
% if isempty(finiteTTC)
%     ylim([0 10]);
% else
%     ylim([0, min(10,max([10; finiteTTC]))]);
% end
% ylabel('TTC (s)'); title('5) TTC');
% 
% % 图6 轨迹
% nexttile; hold on; grid on; box on;
% if hasEgoXY && hasLeadXY
%     plot(S.egoXY(:,1),  S.egoXY(:,2),  'b','LineWidth',1.5,'DisplayName','Ego');
%     plot(S.leadXY(:,1), S.leadXY(:,2), 'r','LineWidth',1.2,'DisplayName','Lead');
%     axis equal;
%     xlabel('X'); ylabel('Y');
%     title('6) 轨迹图');
%     legend('Location','best');
% else
%     text(0.1,0.5,'轨迹不可用：请检查 egoX/egoY/leadX/leadY 是否已记录', 'FontSize',11);
%     axis off;
% end
% 
% saveas(fig, fullfile(saveDir,'Analysis_Report.png'));

%% 6) 导出统计 + JSON
fprintf('>>> 导出统计...\n');

Stats.Timestamp = char(timestamp);
Stats.MAE_Dist = MAE;
Stats.RMSE_Dist = RMSE;
Stats.MinDist_Truth = dmin_truth;
Stats.MinDist_Radar = dmin_radar;
Stats.MinTimeGap = tgMin;
Stats.AEB_Trigger_Count = aebTrig;
Stats.FB_Trigger_Count = fbTrig;
Stats.Hold_Ratio = holdRatio;
Stats.BrakeOverride_Ratio = brkOverrideRatio;
Stats.AccelCut_Ratio = accCutRatio;

save(fullfile(saveDir,'Analysis_Data.mat'),'S','Stats','d_xy','err_dist','-v7.3');

dt = mean(diff(S.time),'omitnan');
if ~isfinite(dt) || dt <= 0, dt = 0.05; end
step = max(1, floor(0.5 / dt));
idx = 1:step:numel(S.time);

J.time        = S.time(idx);
J.v_ego       = S.v_ego(idx);
J.d_rel_truth = S.d_rel_truth(idx);
J.d_rel_radar = S.d_rel_radar(idx);
J.v_rel_truth = S.v_rel_truth(idx);
J.v_rel_radar = S.v_rel_radar(idx);
J.acc_post    = S.acc_post(idx);
J.brk_post    = S.brk_post(idx);
J.holdActive  = S.holdActive(idx);
J.aebModeOut  = S.aebModeOut(idx);
J.TTCOut      = S.TTCOut(idx);
J.cipv_valid  = S.cipv_valid(idx);
J.dUseOut     = S.dUseOut(idx);
J.vrelUseOut  = S.vrelUseOut(idx);

% 轨迹导出（新命名）
J.egoX = S.egoX(idx);
J.egoY = S.egoY(idx);
J.leadX = S.leadX(idx);
J.leadY = S.leadY(idx);

J.steer_pre  = S.steer_pre(idx);
J.steer_post = S.steer_post(idx);

J.stats = Stats;

jsonStr = jsonencode(J,'PrettyPrint',false);
fid = fopen(fullfile(saveDir,'SimLog.json'),'w');
if fid > 0
    fwrite(fid, jsonStr, 'char');
    fclose(fid);
end

fprintf('>>> ✅ 完成: %s\n', saveDir);

end

%% ================= 辅助函数 =================

function logs = get_log_container(out)
% 兼容 SimulationOutput / 直接传日志对象
if isa(out,'Simulink.SimulationOutput')
    try
        logs = out.LogData;  % 你当前模型常用
        return;
    catch
    end
    try
        logs = out.logsout;
        return;
    catch
    end
    error('SimulationOutput 中未找到 LogData/logsout。');
else
    logs = out;
end
end

function t = get_master_time(logs, aliasGroups)
t = [];
for i = 1:numel(aliasGroups)
    names = aliasGroups{i};
    for k = 1:numel(names)
        ts = get_signal_ts(logs, names{k});
        if ~isempty(ts) && numel(ts.Time) > 1
            t = ts.Time(:);
            return;
        end
    end
end
end

function y = get_sig(logs, aliases, tRef, method)
% 按别名依次尝试取信号
for i = 1:numel(aliases)
    ts = get_signal_ts(logs, aliases{i});
    if ~isempty(ts)
        y = safe_resample(ts, tRef, method);
        if isvector(y), y = y(:); end
        return;
    end
end
y = nan(numel(tRef),1);
end

function ts = get_signal_ts(logs, name)
% 从 Dataset / struct 里拿 timeseries
ts = [];
try
    if isa(logs,'Simulink.SimulationData.Dataset')
        % 先直接按名字拿
        try
            el = logs.getElement(name);
            ts = element_to_ts(el);
            if ~isempty(ts), return; end
        catch
        end
        % 再遍历模糊匹配（防 element 名字异常）
        for i = 1:logs.numElements
            el = logs.getElement(i);
            nm = '';
            if isprop(el,'Name'), nm = el.Name; end
            if iscell(nm), nm = nm{1}; end
            if strcmpi(norm_name(nm), norm_name(name))
                ts = element_to_ts(el);
                if ~isempty(ts), return; end
            end
        end

    elseif isstruct(logs)
        % 顶层字段
        if isfield(logs,name)
            ts = value_to_ts(logs.(name));
            if ~isempty(ts), return; end
        end
        % 一层嵌套 struct
        fns = fieldnames(logs);
        for i = 1:numel(fns)
            g = logs.(fns{i});
            if isstruct(g) && isfield(g,name)
                ts = value_to_ts(g.(name));
                if ~isempty(ts), return; end
            end
        end
    end
catch
    ts = [];
end
end

function ts = element_to_ts(el)
ts = [];
try
    if isprop(el,'Values')
        v = el.Values;
        ts = value_to_ts(v);
    end
catch
    ts = [];
end
end

function ts = value_to_ts(v)
ts = [];
if isa(v,'timeseries')
    ts = v; return;
end
if isstruct(v) && isfield(v,'Time') && isfield(v,'Data')
    ts = timeseries(v.Data, v.Time); return;
end
end

function out_data = safe_resample(ts_obj, target_time, method)
if isempty(ts_obj) || numel(ts_obj.Time) < 2
    out_data = nan(numel(target_time),1);
    return;
end
try
    t_raw = ts_obj.Time(:);
    d_raw = ts_obj.Data;

    % 如果是 N×1×M，压成 N×M
    if ndims(d_raw) == 3 && size(d_raw,2) == 1
        d_raw = squeeze(d_raw);
    end

    % 修正 Data 维度方向（3xN -> Nx3）
    if size(d_raw,1) ~= numel(t_raw) && size(d_raw,2) == numel(t_raw)
        d_raw = d_raw.';
    end

    [t_unique, idx] = unique(t_raw, 'stable');
    d_unique = d_raw(idx,:);

    if strcmpi(method,'nearest') || strcmpi(method,'zoh')
        out_data = interp1(t_unique, d_unique, target_time, 'nearest', 'extrap');
    else
        out_data = interp1(t_unique, d_unique, target_time, 'linear', NaN);
    end
catch
    out_data = nan(numel(target_time),1);
end
end

function S = normalize_shapes(S)
fns = fieldnames(S);
N = numel(S.time);
for i = 1:numel(fns)
    k = fns{i};
    v = S.(k);
    if isnumeric(v)
        if isvector(v)
            S.(k) = v(:);
        else
            % 若是 3xN，转成 Nx3
            if size(v,2) == N && size(v,1) ~= N
                S.(k) = v.';
            end
            % 若是 N×1×M，压成 N×M
            if ndims(S.(k)) == 3 && size(S.(k),2) == 1
                S.(k) = squeeze(S.(k));
            end
        end
    end
end
end

function [xy, ok] = build_xy_track(xSig, ySig, tSig)
% 优先使用 x/y；否则从 tSig 取前两列
xy = [];
ok = false;

if ~isempty(xSig) && ~isempty(ySig)
    x = xSig(:); y = ySig(:);
    m = isfinite(x) & isfinite(y);
    if any(m)
        xy = [x, y];
        ok = true;
        return;
    end
end

if ~isempty(tSig) && isnumeric(tSig)
    if ndims(tSig) == 3 && size(tSig,2) == 1
        tSig = squeeze(tSig);
    end
    if ismatrix(tSig) && size(tSig,2) >= 2
        xy = tSig(:,1:2);
        ok = any(isfinite(xy(:)));
    end
end
end

function x = fill_nan_with(x, val)
x(~isfinite(x)) = val;
end

function s = norm_name(x)
if isstring(x), x = char(x); end
if isempty(x), s = ''; return; end
s = lower(char(x));
s = strrep(s,'{','');
s = strrep(s,'}','');
s = strrep(s,'''','');
s = strtrim(s);
end