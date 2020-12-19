% Create an MPC object to be used by the MPC controller.
%
% Input
%   linsys: ss (State-space model) used for the internal model. Acquire
%           this by doing
%               >> linsys = linearize_model(12000);
%   reg:    Struct containing control parameters. Need this because the
%           function cannot read from workspace.
%
% Output
%   lmpc:   mpc (Model predictive controller) object that is tuned and ready
%           to be used by the controller block.

function [lmpc] = config_mpc(linsys, reg)

    % Specify the indices of the various variables.
    iMV = [1 2 3 4];            % aII, aI, grate, waste
    iMD = [];                   % No measured disturbances
    iUD = [];                   % No unmeasured disturbances
    iMO = [1 2];                % Oxygen, Steam
    iUO = [];                   % No unmeasured outputs
    linsys = setmpcsignals(linsys,...
        'MV', iMV, 'MD', iMD, 'UD', iUD, 'MO', iMO, 'UO', iUO);
    
    % Set direct MV feedthrough to zero as this is required by the MPC.
    linsys.D(:,iMV) = 0;

    % Define the span of the variables. This is used to scale the variable
    % changes into comparable values. See the Simulink documentation for
    % more information.
    uSpan = [2.8 3.6 0.0005 0.8];   % aII, aI, grate, waste
    ySpan = [5.0, 5.0];             % Oxygen, steam
    
    Ts = 20;    % Sampling time
    p = 50;     % Prediction horizon
    m = 2;      % Control horizon
    
    % Create the MPC
    lmpc = mpc(linsys, Ts);
    lmpc.PredictionHorizon = p;
    lmpc.ControlHorizon = m;
    
    % Set weights
    wMV = [0 0 0 0];            % MV target punishment
    tMV = [0 0 0 0];            % MV targets (relative to operating point)
    wMVR = [0.1 0.1 0.1 0.1];   % Punish MV derivatives
    wOV = [1 3];                % Punish CV setpoint deviation
    
    for i=1:4
        lmpc.MV(iMV(i)).ScaleFactor = uSpan(i);
        lmpc.MV(iMV(i)).Target = tMV(i);
        lmpc.Weights.MV(iMV(i)) = wMV(i);
        lmpc.Weights.MVRate(iMV(i)) = wMVR(i);
        lmpc.MV(iMV(i)).Min = -1 * reg.op_MVs(i);
    end
    for i=1:2
        lmpc.OV(iMO(i)).ScaleFactor = ySpan(i);
        lmpc.Weights.OV(iMV(i)) = wOV(i);
    end
    
    % Explicitly set the output disturbance model to a step function.
    % This should apparently be the default behavious, but it isn't.
    outDist_O2 = tf([5], [1 0]);
    outDist_st = tf([5], [1 0]);
    outDist = [outDist_O2 0; 0 outDist_st];
    setoutdist(lmpc, 'model', outDist);
end

