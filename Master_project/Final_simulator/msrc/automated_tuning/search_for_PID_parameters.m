%% Preparation
run prepare_furnce
global approxomated_system
global u_star
global constant_response
global k
global reg
global mpc_function_type
global mpc_params
FurnaceParameters
%
% reg.input_type_Q_grate = InputType.Experimental;
% reg.input_type_T_aI = InputType.Experimental;
% reg.input_type_T_aII = InputType.Experimental;

%% Actual code
initial_attempt = [
                reg.Kp_O2;
                reg.Kp_st;
                reg.Kp_AB;
                reg.Kp_HHV;
                reg.Kp_st_HHV;
                reg.Kp_AB_HHV;
                reg.Kp_fan;
                reg.Ki_O2;
                reg.Ki_st;
                reg.Ki_AB;
                reg.Ki_HHV;
                reg.Ki_st_HHV;
                reg.Ki_AB_HHV;
                reg.Ki_fan;
                reg.Kp_O2;
                reg.Kp_st;
                reg.Kp_AB;
                reg.Kp_HHV;
                reg.Kp_st_HHV;
                reg.Kp_AB_HHV;
                reg.Kp_fan;
                ];
%%
% initial_attempt = [9.864806e-01, 5.773660e+01, -1.600771e-03, 1.426883e-07, 1.494721e+07, 2.664931e-04, 2.326215e+01, 1.367848e+00, 9.091916e-01, -1.969125e-05, 5.849626e-08, -1.521913e+02, -3.629613e-05, 3.640847e+01, 2.887300e+00, -4.969528e+01, -6.612263e-04, 3.159532e-07, 4.071125e+06, -1.930806e-03, 1.829843e+01];
test_PID(initial_attempt)
%% Search

options = optimset('PlotFcns', @optimplotfval);
best_params = fminsearch(@test_PID, initial_attempt, options)
disp("Optimization done")
%% Re-run
[cost, best_reg] = test_PID(best_params);
"DONE"
%% Function
function [cost, reg] = test_PID(pid_parameters_vector)
    global reg
    global mpc_params
    global mpc_function_type
    global k

    reg.controller_type = ControllerType.Cascade;

    % Activate disturbances
    reg.input_type_Q_grate = InputType.Experimental;
    reg.input_type_T_aI = InputType.Experimental;
    reg.input_type_T_aII = InputType.Experimental;
    %% Controller parameters
    % % Controller prime components
    reg.Kp_O2 = pid_parameters_vector(1);
    reg.Kp_st = pid_parameters_vector(2);
    reg.Kp_AB = pid_parameters_vector(3);
    reg.Kp_HHV = pid_parameters_vector(4);
    reg.Kp_st_HHV = pid_parameters_vector(5);
    reg.Kp_AB_HHV = pid_parameters_vector(6);
    reg.Kp_fan = pid_parameters_vector(7);

    % % Controller integrator components
    reg.Ki_O2 = pid_parameters_vector(8);
    reg.Ki_st = pid_parameters_vector(9);
    reg.Ki_AB = pid_parameters_vector(10);
    reg.Ki_HHV = pid_parameters_vector(11);
    reg.Ki_st_HHV = pid_parameters_vector(12);
    reg.Ki_AB_HHV = pid_parameters_vector(13);
    reg.Ki_fan = pid_parameters_vector(14);

    %% Derivative componenet
    reg.Kd_O2 = pid_parameters_vector(15);
    reg.Kd_st = pid_parameters_vector(16);
    reg.Kd_AB = pid_parameters_vector(17);
    reg.Kd_HHV = pid_parameters_vector(18);
    reg.Kd_st_HHV = pid_parameters_vector(19);
    reg.Kd_AB_HHV = pid_parameters_vector(20);
    reg.Kd_fan = pid_parameters_vector(21);
    %% Simulation and cost
    try
        simulation = sim("Furnace");

        try
            simout_F_st = simulation.simout_F_st; % Some simulation give the return values in a wierd way
            simout_F_aI = simulation.simout_F_aI;
            simout_F_aII = simulation.simout_F_aII;
            simout_F_O2 = simulation.simout_F_O2;
        catch
            % Sometimes the to workspace variables are from the sim return
            % value. Sometimes they are added directly to the workspace
        end

        % Calculate costs
        steam_error = squeeze((abs(resample(simout_F_st, simout_F_st.time(1):1:simout_F_st.time(end)).data) - reg.sp_F_st));
        O2_error = squeeze((abs(resample(simout_F_O2, simout_F_O2.time(1):1:simout_F_O2.time(end)).data) - reg.sp_F_O2));
        air_diff_error = (squeeze((abs(resample(simout_F_aI - simout_F_aII, simout_F_O2.time(1):1:simout_F_O2.time(end)).data) - (reg.op_F_aI - reg.op_F_aII))));
        overshoot_cost = (norm(steam_error, Inf) + 10 * norm(O2_error, Inf) + 0.01 * norm(air_diff_error, Inf));
        L2_norm_cost = ((norm(steam_error, 2) + 50 * norm(O2_error, 2) + 0.5 * norm(air_diff_error, 2)) / length(steam_error));
        const_error_cost = (0.01 * abs(sum(steam_error)) + abs(sum(O2_error))) / length(steam_error); % Punish if the system does not converge to reference properly
        sim_cost = overshoot_cost + L2_norm_cost + const_error_cost;
        cost = sim_cost;
        save("outputs/current_PID_autotuned_params", "pid_parameters_vector", "reg")
        %         try
        %             simout_F_st = simulation.simout_F_st;
        %             simout_F_O2 = simulation.simout_F_O2;
        %         catch
        %
        %         end
        %         steam_error = (abs(resample(simout_F_st, simout_F_st.time(1):1:simout_F_st.time(end)).data - reg.sp_F_st));
        %         O2_error =  (abs(resample(simout_F_O2, simout_F_O2.time(1):1:simout_F_O2.time(end)).data - reg.sp_F_O2));
        %         overshoot_cost = (0.1*norm(steam_error, Inf) + norm(O2_error, Inf));
        %         L2_norm_cost  = ( norm(steam_error,2) + 10*norm(O2_error,2) /length(steam_error)) ;
        %         cost = overshoot_cost + L2_norm_cost;
    catch ME
        ME
        cost = 1e8; % A big punishment for unstability
    end

    fprintf("Cost: %i <- %s\n", cost, sprintf("%i, ", pid_parameters_vector))
end
