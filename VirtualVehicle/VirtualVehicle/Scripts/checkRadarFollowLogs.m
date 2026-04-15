function summary = checkRadarFollowLogs(out)
%CHECKRADARFOLLOWLOGS Inspect signals needed for radar-follow debugging.
%   summary = CHECKRADARFOLLOWLOGS(simout)
%   summary = CHECKRADARFOLLOWLOGS("simout.mat")
%   summary = CHECKRADARFOLLOWLOGS()
%
% The function checks whether the simulation output contains the key log
% branches that are usually needed to debug a radar-based following stack:
% ego state, ground truth target, tracker output, and the final selected
% target used by the algorithm.

    if nargin < 1 || isempty(out)
        out = loadDefaultInput();
    elseif isstring(out) || ischar(out)
        out = loadFromMatFile(char(out));
    end

    logs = extractLogs(out);

    checks = {
        'EgoState.X'
        'EgoState.Y'
        'EgoState.Xdot'
        'EgoState.Ydot'
        'TargetState.Location'
        'FunctionOutput.rel_dist'
        'FunctionOutput.rel_vel'
        'FunctionOutput.target_id'
        'TrackerOutput.Tracks'
    };

    summary = struct();
    summary.availableTopLevel = fieldnames(logs);
    summary.results = repmat(struct('Path', '', 'Exists', false, 'Class', '', 'Detail', ''), numel(checks), 1);

    fprintf('\n=== Radar Follow Log Check ===\n');
    fprintf('Top-level log branches:\n');
    for i = 1:numel(summary.availableTopLevel)
        fprintf('  - %s\n', summary.availableTopLevel{i});
    end
    fprintf('\nRequired debug signals:\n');

    for i = 1:numel(checks)
        [existsFlag, value] = tryGetPath(logs, checks{i});
        summary.results(i).Path = checks{i};
        summary.results(i).Exists = existsFlag;

        if existsFlag
            summary.results(i).Class = class(value);
            summary.results(i).Detail = describeValue(value);
            fprintf('[OK  ] %-28s  %s\n', checks{i}, summary.results(i).Detail);
        else
            summary.results(i).Class = '';
            summary.results(i).Detail = 'missing';
            fprintf('[MISS] %-28s  missing\n', checks{i});
        end
    end

    fprintf('\nQuick interpretation:\n');
    printHint(summary.results, 'TargetState.Location', ...
        'No ground-truth target log. You cannot compare radar output against the true lead vehicle.');
    printHint(summary.results, 'FunctionOutput.rel_dist', ...
        'No selected target distance. The target-selection or follow algorithm output is not logged.');
    printHint(summary.results, 'FunctionOutput.rel_vel', ...
        'No selected target relative speed. ACC/follow control cannot be validated yet.');
    printHint(summary.results, 'TrackerOutput.Tracks', ...
        'No raw tracker tracks. You can only debug the final output, not the upstream detection/tracker stage.');

    fprintf('\nNext step:\n');
    fprintf('1. Make sure the model can run once.\n');
    fprintf('2. Run this checker.\n');
    fprintf('3. If any item is missing, add logging on that branch before tuning the controller.\n\n');
end

function out = loadDefaultInput()
    if evalin('base', 'exist(''simout'', ''var'')')
        out = evalin('base', 'simout');
        return;
    end

    if isfile('simout.mat')
        out = loadFromMatFile('simout.mat');
        return;
    end

    error(['No input provided. Pass a SimulationOutput object, or place ', ...
           'simout in the base workspace, or keep simout.mat in the current folder.']);
end

function out = loadFromMatFile(filePath)
    data = load(filePath);
    names = fieldnames(data);

    if isfield(data, 'simout')
        out = data.simout;
        return;
    end

    if numel(names) == 1
        out = data.(names{1});
        return;
    end

    error('MAT file "%s" does not contain a clear simulation output variable.', filePath);
end

function logs = extractLogs(out)
    if isstruct(out) && isfield(out, 'LogData')
        logs = out.LogData;
        return;
    end

    if isobject(out)
        if isprop(out, 'LogData')
            logs = out.LogData;
            return;
        end
    end

    error(['Unsupported simulation output format. Expected an object or struct ', ...
           'with a LogData field.']);
end

function [existsFlag, value] = tryGetPath(rootValue, dottedPath)
    parts = strsplit(dottedPath, '.');
    value = rootValue;
    existsFlag = true;

    for i = 1:numel(parts)
        name = parts{i};
        if isstruct(value)
            if ~isfield(value, name)
                existsFlag = false;
                value = [];
                return;
            end
            value = value.(name);
        else
            try
                value = value.(name);
            catch
                existsFlag = false;
                value = [];
                return;
            end
        end
    end
end

function detail = describeValue(value)
    if isa(value, 'timeseries')
        detail = sprintf('timeseries, %d samples', numel(value.Time));
        return;
    end

    if isnumeric(value) || islogical(value)
        sz = size(value);
        detail = sprintf('%s [%s]', class(value), joinSize(sz));
        return;
    end

    if isstruct(value)
        detail = sprintf('struct with %d fields', numel(fieldnames(value)));
        return;
    end

    if isa(value, 'Simulink.SimulationData.Dataset')
        detail = sprintf('Dataset with %d elements', value.numElements);
        return;
    end

    detail = class(value);
end

function out = joinSize(sz)
    parts = arrayfun(@num2str, sz, 'UniformOutput', false);
    out = strjoin(parts, 'x');
end

function printHint(results, pathText, message)
    idx = find(strcmp({results.Path}, pathText), 1);
    if isempty(idx)
        return;
    end

    if ~results(idx).Exists
        fprintf('- %s\n', message);
    end
end
