% Runs a simulation from an earlier state.
%
% Input
%   ss_state: ModelOperatingPoint representing a steady state.
%   run_time: Integer number of seconds for the simulation to run.
%
% Output
%   simOut: SimulationOutput containing simulation data. Note that the
%           content of this struct is determined by configuration set in
%           Model Configuration Properties -> Data Import/Export.
%
function [simOut] = sim_from_ss(ss_state, run_time)

    ss_time = ss_state.snapshotTime;
    stop_time = ss_time + run_time;
    set_param(...
        'Furnace',...
        'LoadInitialState', 'on',...
        'InitialState', 'ss_state'...
    );

    tic
    simOut = sim(...
        'Furnace',...
        'StartTime', sprintf('%d', ss_time),...
        'StopTime', sprintf('%d', stop_time)...
    );
    toc
end
