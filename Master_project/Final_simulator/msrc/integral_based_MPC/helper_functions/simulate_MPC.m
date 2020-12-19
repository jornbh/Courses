function [Linv] =simulate_MPC(disc_MPC_sys, physical_ref, H, F_y_ref, F_du, F_x0, Ac,b0, disc_integrator_B)
    %% Performs a simplified simulation of the MPC 
    % Also returns Linv, which can be used when omtimizing the QP needed by the MPC (Faster when many iterations are performed)


    % Create shorthand names for some variables 
    A = disc_MPC_sys.A;
    B = disc_MPC_sys.B;
    C = disc_MPC_sys.C;
    MPC_dt = disc_MPC_sys.Ts; 
    N_prediction_steps = fix(length(H)/size(B,2)); % This only applies if no slack variables are used
    % Pre-prepared version of H (Mathemagics)
        % The mpcActiveSetSolver function requires the first input to be the inverse of the lower-triangular Cholesky decomposition of the Hessian matrix H.(?)
    L = chol(H, 'lower');
    Linv = L \ eye(size(H, 1));




    % Initialize the states the MPC
    x0 = ones([size(A, 1) 1]);
    x0(1:10) = 100; % Start with some excitation 
    x = x0; %Initial state
    iA = false(size(b0)); % true => active constraint (=> solve QP faster)
    MPC_opt = mpcActiveSetOptions;
    MPC_opt.IntegrityChecks = false;
    MPC_opt.UseHessianAsInput = false; % => Linv is used instead of H => Faster 
    
    time = 0:MPC_dt:4e5;
    C_sim = C(1:fix(size(C, 1) / 2), :); % => Do not plot integrator values
    yMPC = zeros([length(time), size(C_sim, 1), ]);
    uMPC = zeros([length(time), size(B, 2)]);

    
    u_prev = zeros([size(B, 2) * N_prediction_steps, 1]);

    
    %% Simulate
    for time_ind = 1:length(time)
        new_b0 = b0; %#TODO Handle state constraints
%         F_y_ref, F_du, F_x0
        Corrected_F = (F_x0*x+F_y_ref*physical_ref+F_du*u_prev);
        [u, status, iA] = mpcActiveSetSolver(Linv, Corrected_F , Ac, new_b0, zeros(0, N_prediction_steps), zeros(0, 1), iA, MPC_opt);

        % u = -K_lqr*x; % Use lqr instead
        uMPC(time_ind, :) = u(1:size(B, 2)); % TODO: Something is wrong about the
        u_prev = u;

        x = A * x + B * uMPC(time_ind, :)' + disc_integrator_B * physical_ref;
        yMPC(time_ind, :) = C_sim * x;
    end


    % Plot inputs and outputs
    figure
    subplot(2, 1, 1)
    hold on
    plot(time, uMPC(:, 1))
    plot(time, uMPC(:, 2))
    plot(time, uMPC(:, 3))
    plot(time, uMPC(:, 4))
    hold off
    legend("F_{aII}", "Waste flow", "A primary air (mean of all)", "Grate speed")
    xlabel('time')
    ylabel('u')
    subplot(2, 1, 2)
    plot(time, yMPC, "x-")
    xlabel('time')
    ylabel('y')
    legend('Constrained MPC F_{st}', "Y_{O2}", "\int \Delta F_{st}", "\int \Delta Y_{O2}")% , "T_fg_out", "T_fg")

end
