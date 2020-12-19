% Runs the simulation to a steady state and creates a linear state-space
% model around the operating point.
%
% Input
%   ss_time: Integer number of seconds at which startup is finished and the
%            system is in a steady state.
%
% Output
%   linsys: ss (State-space model) created using finite differences at the
%           steady state operating point.
%
function [linsys] = linearize_model(ss_time)

    % Get current configuration
    evalin('caller', 'FurnaceParameters;');
    
    % Set all input types to constant.
    suffixes = ["F_aI" "F_aII" "F_w_in" "v_grate" "HHV" "AB"];
    for i = 1:length(suffixes)
        evalin('caller',...
            sprintf("reg.input_type_%s = InputType.Constant;", suffixes(i)));
    end

    set_param(...
        'Furnace',...
        'LoadInitialState', 'off',...
        'StartTime', '0',...
        'StopTime', sprintf('%d', ss_time)...
    );

    % Create an operating point. We cannot use an existing ModelOperatingPoint
    % because the linearization process itself temporarily changes the model 
    % inputs/outputs and as such invalidates the ModelOperatingPoint.
    op = findop('Furnace', ss_time);

    % Define the inputs and outputs of the linear model. Note that the I/O
    % points set in the interactive editor are not used, and that if the
    % signal names change, the following will break.
    clearvars io;
    io(1) = linio(strcat('Furnace', '/Secondary air/const_F_aII'), 1, 'input');
    io(2) = linio(strcat('Furnace', '/Waste and Primary air/const_F_aI'), 1, 'input');
    io(3) = linio(strcat('Furnace', '/Grate speed/const_v_grate'), 1, 'input');
    io(4) = linio(strcat('Furnace', '/Waste and Primary air/const_F_w_in'), 1, 'input');
    io(5) = linio(strcat('Furnace', '/Controller/F_O2_in'), 1, 'openoutput');
    io(6) = linio(strcat('Furnace', '/Controller/F_st_in'), 1, 'openoutput');
    
    % Perform the linearization
    linsys = linearize('Furnace', io, op);

    % Set proper names
    linsys.InputName = {'F_{aII}';'F_{aI}';'v_{grate}';'F_{w}'};
    linsys.OutputName = {'F_{O2}';'F_{st}'};
    
    % Plot the resulting linearization step responses
    h = stepplot(linsys);
    setoptions(h, 'Normalize', 'on');
end

