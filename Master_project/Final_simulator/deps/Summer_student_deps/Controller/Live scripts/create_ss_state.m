% Create a steady state from scratch.
%
% Input
%   ss_time: Integer number of seconds to run until the steady state
%            is acquired.
%
% Output
%   ss_state: ModelOperatingPoint containing the steady state.
%   simOut:   SimulationOutput containing simulation data. Note that the
%             content of this struct is determined by configuration set in
%             Model Configuration Properties -> Data Import/Export.

function [ss_state, simOut] = create_ss_state(ss_time)

    set_param(...
        'Furnace',...
        'LoadInitialState', 'off',...
        'SaveOperatingPoint','on',...
        'SaveFinalState', 'on',...
        'FinalStateName', 'ss_state_out'...
    );

    % Get current configuration
    evalin('caller', 'FurnaceParameters;');
    
    % Set all input types to constant
    suffixes = ["F_aI" "F_aII" "F_w_in" "v_grate" "AB" "HHV"];
    for i = 1:length(suffixes)
        evalin('caller',...
            sprintf("reg.input_type_%s = InputType.Constant;", suffixes(i)));
    end

    % Simulate
    tic
%     simOut = sim(...
%         'Furnace',...
%         'StartTime', '0',...
%         'StopTime', sprintf('%d', ss_time)...
%     );

    simOut = sim(...
        'Furnace',...
        'StopTime', sprintf('%d', ss_time)...
    );
    toc
    
    % Reset configuration
    evalin('caller', 'FurnaceParameters;');
    ss_state = simOut.ss_state_out;
end

