
%%
if ~exist("simulink_model_name", "var")
    simulink_model_name = "sandbox";
    MPC_type = MPCType.LQRDiffIntegralAgnostic;
    MPC_name_prefix = "LQRDiffIntegralAgnostic"; 
end

%% Prepare furnace
[original_controller_params_struct, original_lqe_params_struct]  = create_MPC(); 


if simulink_model_name == "Furnace"
    FurnaceParameters
    disp("Computing ss_state instead");
%     set_param('Furnace/Controller/MPC_subsystem/LQG_controller/x_hat_selector', "sw", "0"); % Disconnect MPC
    ss_state = create_ss_state(15e3);
%     set_param('Furnace/Controller/MPC_subsystem/LQG_controller/x_hat_selector', "sw", "1"); % Disconnect MPC

    FurnaceParameters;
    set_param("Furnace", "LoadInitialState", "on", ...
        "StopTime", "23000"); % Removes some of the manual work
    "Summer project initialized"
else % Initialize sandbox
    FurnaceParameters;
    ss_state = []; % The closest thing to a null in Matlab...
end

%%

use_disturbance_measurements = false;

reg.enforce_parameter_limits = true;
reg.controller_type = ControllerType.MPC;
% An old initial attempt to get good controllers

% R =diag([
%         1e-6;  % Secondary air
%         1e-3;   % Waste-flow (Ram speed(?))
%         1e-6;  % Primary air flow 1
%         1e3;  % Grate speed (Very sensitive)
%         ]*1e4);
% % Greatly punish the two kinds of air for going in the oposite direction
% air_diff_cost = 1e-9;
% air_ratio = 0.9; % Primary to secondary
% R(1,1) = R(1,1) +air_diff_cost;
% R(3,3) = R(3,3) +air_diff_cost*air_ratio^2;
% R(1,3) =        -air_diff_cost*air_ratio;
% R(3,1) =        -air_diff_cost*air_ratio;
%
%
% Rd = diag([
%         2e-6;  % Secondary air
%         1e0;   % Waste-flow (Ram speed(?))
%         3e-6;  % Primary air flow 1
%         1e3;  % Grate speed (Very sensitive)
%         ]*1e3);
% Qy = 1e-9*diag([1e4,1e3,1e7, 1e4]); % F_st, Y_O2, and the integrated versions
% Qy_diff  = 1e-25*diag([1e2,1e-12]);
%
% % Q_input_disturbances = pinv(approxomated_system.B)'*diag([1e-2,1e-2,1e-4,1e-2])*pinv(approxomated_system.B);
% % dummy_Q_lqe = dummy_Q_lqe + Q_input_disturbances;
% R_lqe = diag([9.12257e-06,9.20423e-06,1.02345e+01]); %
%
% Q_lqe = diag([1.77872e-06,2.81258e-1,1.46160e+5,2.89662e+5,6.59935e+5,]); %Measured disturbances variances
% Q_lqe_agnostic = diag([9.22903e-05,9.01234e-05,1.05834e-04,9.66810e-05,9.06186e-05,9.17000e-05,9.42873e-05,9.58339e-05,9.50371e-05,9.85408e-05,1.05156e-04,9.75820e-05])
%
% air_diff_integral_cost = 1e-15;
% Qy_extended = blkdiag(Qy(1:2,1:2),0,Qy(3:4,3:4), air_diff_integral_cost);
% SANDBOX
% load("committed_best_LQR_integrated_agnostic_params.mat")% TODO DELETE THIS
% load('best_LQR_integrated_agnostic_params.mat')
load committed_best_MPCDiscreteDiffIntegralAgnostic_params.mat
air_ratio = 1;
% A gentler initial guess is needed when tunin, to avoid an unstable
% initial attempt
Qy = diag([10e-2, 10e-2, 1.0e-1, 1e-1] );
R = diag([1e2, 1.06440e-3, 1e2, 5e+09])
ac = 1e-10;
dair = [ac, 0,-ac;0,0,0; -ac,0, ac];
R(1:3,1:3) = R(1:3,1:3) + dair;
% integral_air_diff_cost = 1e-7;
Rd = diag([2.00000e-02, 1e4, 3.00000e-0, 1e-4,]);
Qy_diff = diag([2.44170e2, 5.92560e2,]);
% R_lqe = R_lqe*1e-4
% Q_lqe = Q_lqe*1e6;
% Qy = Qy*1e1

% Extra parameters 
R_old = R;
Qy_old = Qy; 
R = 1e7* diag([1e2, 1.06440e3, 1e2, 5e+09]);
ac = 1e-10;
Q_lqe = 1e-3*diag([1.23707e-04,  8.49319e-02,  6977898838404436992,  1.82132e+04,  8.06462e+00]); 
Q_lqe_agnostic = eye(12)*1e4; 
R_lqe = diag([1,1,1]);
dair = [ac, 0,-ac;0,0,0; -ac,0, ac];
R(1:3,1:3) = R(1:3,1:3) + dair;
Qy = Qy_old.*diag([1e1,1e1,1e3,1e3])

MPC_name_prefix = "LQRDiffIntegralAgnostic"
MPC_type = MPCType.LQRDiffIntegralAgnostic;
% 
% MPC_name_prefix = "MPCDiscreteDiffIntegralAgnostic"
% MPC_type = MPCType.MPCDiscreteDiffIntegralAgnostic;
load committed_best_LQRDiffIntegralAgnostic_params.mat % The filter seems to be a winner
% R = R*1e-7
R(end,end) = R(end,end)*1e3; % A more carefull controller (For robustness) 
% % R(2,2) = R(2,2)*1e-1; 
Rd = Rd*1e9
% Qy(end-1:end, end-1:end) = Qy(end-1:end, end-1:end)*1e1;
% %\end SANDBOX



initial_attempt = log10((diag(blkdiag(...
    R - diag([R(1, 3), 0, R(1, 3), 0]), ...
    Rd, ...
    Qy, ...
    Qy_diff, ...
    R_lqe, ...
    Q_lqe, ...
    Q_lqe_agnostic, ...
    Qy_extended(6, 6), ...% Integral air diff ratio cost
10^air_ratio, ...
    - R(1, 3) ...
    ))));

%  First run

target_function = make_seach_fun(...
    MPC_name_prefix, ...
    use_disturbance_measurements, ...
    MPC_type, ...
    simulink_model_name, ...
    original_controller_params_struct, ... 
    original_lqe_params_struct, ... 
    ss_state, ...
    Q_grate, ...
    T_aI, ...
    T_aII, ...
    F_aII, ...
    F_aI, ...
    F_w_in, ...
    F_from_DH, ...
    T_from_DH, ...
    reg, ...
    k ...
    )
% @@@
% % Use an empirically better cost instead

options = optimset('PlotFcns', ["optimplotfval", "optimplotfunccount", "optimplotx"]);
set_param(simulink_model_name, 'FastRestart', 'off') %Reset fast restart
set_param(simulink_model_name, 'FastRestart', 'on')
target_function(initial_attempt)
"Ready to optimize"
%% Prepare a figure that can be ploted to

%% Search for the best parameters
set_param(simulink_model_name, 'FastRestart', 'off') %Reset fast restart
set_param(simulink_model_name, 'FastRestart', 'on')
best_params = fminsearch(target_function, initial_attempt, options)
set_param(simulink_model_name, 'FastRestart', 'off')

%% ALternative optimization method
% sim_aneal_options = optimoptions( "simulannealbnd", "PlotFcn", {@saplotbestf, @saplotbestx})
% set_param(simulink_model_name,'FastRestart','on')
% best_params  = simulannealbnd(target_function, initial_attempt, [],[], sim_aneal_options )
% set_param(simulink_model_name,'FastRestart','off')

%% Genetic algoritm
% ga_options = optimoptions( "ga", "PlotFcn", { "gaplotbestf", "gaplotscores"});
% set_param("simulink_model_name",'FastRestart','on')
% best_params = ga(target_function, length(initial_attempt),[],[],[],[],[],[],[], ga_options)
% set_param("simulink_model_name",'FastRestart','off')
%% Test
[cost, R, Rd, Qy, Qy_extended, Qy_diff, Q_lqe, R_lqe] = target_function(best_params);
% [cost, R, Rd, Qy, Qy_diff, Q_lqe, R_lqe] = MPC_search(best_params); % Extract the valueble parameters from the search
%%

function [target_function] = make_seach_fun(...
        MPC_name_prefix, ...
        use_disturbance_measurements, ...
        MPC_type, ...
        simulink_model_name, ...
        original_controller_params_struct, ... 
        original_lqe_params_struct, ... 
        ss_state, ...
        Q_grate, ...
        T_aI, ...
        T_aII, ...
        F_aII, ...
        F_aI, ...
        F_w_in, ...
        F_from_DH, ...
        T_from_DH, ...
        reg, ...
        k ...
        )

    %% Prepare Furnace and parameters if needed
    original_lqe_params = original_lqe_params_struct.lqe_params; % Does not matter if these are shared...
    original_mpc_params = original_lqe_params_struct; % Does not matter if these are shared...
    original_agnostic_lqe_params = original_lqe_params_struct.agnostic_lqe_params; % Does not matter if these are shared...
    
    full_estimated_plant_B =  [original_lqe_params.observed_plant.B, original_lqe_params.G_lqe]; 
    full_estimated_plant_D = [original_lqe_params.observed_plant.D, zeros(3, size(original_lqe_params.G_lqe,2))]
    full_estimated_plant = ss(original_lqe_params.observed_plant.A, full_estimated_plant_B, original_lqe_params.observed_plant.C, full_estimated_plant_D);

    % Copy-paste from
        original_MPCDiscrete_params = original_controller_params_struct.MPCDiscrete_params;
        original_MPCPseudoDiscrete_params = original_controller_params_struct.MPCPseudoDiscrete_params;
        original_LQR_params = original_controller_params_struct.LQR_params;
        original_LQRDiffIntegral_params = original_controller_params_struct.LQRDiffIntegral_params;
        original_MPCDiscreteAgnostic_params = original_controller_params_struct.MPCDiscreteAgnostic_params;
        original_MPCPseudoDiscreteAgnostic_params = original_controller_params_struct.MPCPseudoDiscreteAgnostic_params;
        original_LQRAgnostic_params = original_controller_params_struct.LQRAgnostic_params;
        original_LQRDiffIntegralAgnostic_params = original_controller_params_struct.LQRDiffIntegralAgnostic_params;
        original_MPCDiscreteDiffIntegralAgnostic_params = original_controller_params_struct.MPCDiscreteDiffIntegralAgnostic_params;
        
        original_MPCDiscrete_function_parameters= original_controller_params_struct.MPCDiscrete_function_parameters;
        original_MPCPseudoDiscrete_function_parameters= original_controller_params_struct.MPCPseudoDiscrete_function_parameters;
        original_LQR_function_parameters= original_controller_params_struct.LQR_function_parameters;
        original_LQRDiffIntegral_function_parameters= original_controller_params_struct.LQRDiffIntegral_function_parameters;
        original_MPCDiscreteAgnostic_function_parameters= original_controller_params_struct.MPCDiscreteAgnostic_function_parameters;
        original_MPCPseudoDiscreteAgnostic_function_parameters= original_controller_params_struct.MPCPseudoDiscreteAgnostic_function_parameters;
        original_LQRAgnostic_function_parameters= original_controller_params_struct.LQRAgnostic_function_parameters;
        original_LQRDiffIntegralAgnostic_function_parameters= original_controller_params_struct.LQRDiffIntegralAgnostic_function_parameters;
        original_MPCDiscreteDiffIntegralAgnostic_function_parameters = original_controller_params_struct.MPCDiscreteDiffIntegralAgnostic_function_parameters;

    %% THe used function
    target_function = @(x) long_target_function(x, ...
        MPC_name_prefix, ...
        use_disturbance_measurements, ...
        MPC_type, ...
        simulink_model_name, ...
        original_controller_params_struct, ... 
        original_lqe_params_struct, ... 
        ss_state, ...
        Q_grate, ...
        T_aI, ...
        T_aII, ...
        F_aII, ...
        F_aI, ...
        F_w_in, ...
        F_from_DH, ...
        T_from_DH, ...
        reg, ...
        k ...
        )

    function [cost, R, Rd, Qy, Qy_extended, Qy_diff, Q_lqe, R_lqe] = long_target_function(...
            parameters, ...
            MPC_name_prefix, ...
            use_disturbance_measurements, ...
            MPC_type, ...
            simulink_model_name, ...
            original_controller_params_struct, ... 
            original_lqe_params_struct, ... 
            ss_state, ...
            Q_grate, ...
            T_aI, ...
            T_aII, ...
            F_aII, ...
            F_aI, ...
            F_w_in, ...
            F_from_DH, ...
            T_from_DH, ...
            reg, ...
            k ...
            )

        %         FurnaceParameters
        Nx_agnostic = size(original_agnostic_lqe_params.observed_plant.A, 1);
        Nu = size(original_lqe_params.observed_plant.B, 2);

        Ny_obsv = 3; 
        Ny_controlled = 2;
        Ny_diff = 3; 
        % Unpack all the parameters into the diagonals for the weight matrices
        i = 1;
        R_base = parameters(i:(i + Nu - 1));
        i = i + length(R_base);
        Rd_base = parameters(i:(i + Nu - 1));
        i = i + length(Rd_base);
        Qy_base = parameters(i:(i + Ny_controlled*2 - 1));
        i = i + length(Qy_base);
        Q_y_diff_base = parameters(i:(i + Ny_controlled - 1));
        i = i + length(Q_y_diff_base);
        R_lqe_base = parameters(i:(i + Ny_obsv - 1));
        i = i + length(R_lqe_base);
        Q_lqe_base = parameters(i:(i +5) -1);
        i = i + length(Q_lqe_base);
        Q_lqe_agnostic_base = parameters(i:(i + Nx_agnostic - 1));
        i = i + length(Q_lqe_agnostic_base);
        air_diff_integral_cost = 10^parameters(i);
        i = i + 1;
        alpha = parameters(i);
        i = i + 1;
        diff_cost = parameters(i); % Choose a scaling between the two air-flows

        %%
        R = diag(10.^R_base);
        R(1, 1) = R(1, 1) +(10.^(diff_cost));
        R(3, 3) = R(3, 3) +(10.^(diff_cost)) * alpha^2;
        R(1, 3) = -(10.^(diff_cost)) * alpha;
        R(3, 1) = -(10.^(diff_cost)) * alpha;
        Rd = diag(10.^Rd_base);
        Qy = diag(10.^Qy_base);
        Qy_extended = blkdiag(Qy(1:2, 1:2), 0, Qy(3:4, 3:4), air_diff_integral_cost);
        Qy_diff = diag(10.^Q_y_diff_base);
        Q_lqe = diag(10.^Q_lqe_base);
        R_lqe = diag(10.^R_lqe_base);
        Q_lqe_agnostic = diag(10.^Q_lqe_agnostic_base);

        %% Use the weights to make the mpc_params

        weights.R               = R               ;  
        weights.Rd              = Rd              ;  
        weights.Qy              = Qy              ;  
        weights.Qy_extended     = Qy_extended     ;          
        weights.Qy_diff         = Qy_diff         ;      
        weights.Q_lqe           = Q_lqe           ;      
        weights.R_lqe           = R_lqe           ;      
        weights.Q_lqe_agnostic  = Q_lqe_agnostic  ;              

        try
            %% Copy of create_MPC
            %             load('disturbance_estimation_experiment_result.mat')

            [controller_params_struct, lqe_params_struct] = create_MPC(...
            weights, ...
            original_lqe_params , ... % Optional params
            original_agnostic_lqe_params,...
            [MPC_name_prefix]);

            % Set lqe params 
            lqe_params = lqe_params_struct.lqe_params;
            agnostic_lqe_params = lqe_params_struct.agnostic_lqe_params;
            %% Assign all the controller parameters
            %suffixes = split(sprintf("%s;",MPCType(1:9)),";")
            %suffixes = suffixes(1:end-1)
            %fprintf("%s;\n", suffixes + "_params = original_controller_params_struct." +  suffixes + "_params")
%             fprintf("%s;\n", suffixes + "_function_parameters= original_controller_params_struct." +  suffixes + "_function_parameters")
            % Copy-paste
            MPCDiscrete_params = original_controller_params_struct.MPCDiscrete_params;
            MPCPseudoDiscrete_params = original_controller_params_struct.MPCPseudoDiscrete_params;
            LQR_params = original_controller_params_struct.LQR_params;
            LQRDiffIntegral_params = original_controller_params_struct.LQRDiffIntegral_params;
            MPCDiscreteAgnostic_params = original_controller_params_struct.MPCDiscreteAgnostic_params;
            MPCPseudoDiscreteAgnostic_params = original_controller_params_struct.MPCPseudoDiscreteAgnostic_params;
            LQRAgnostic_params = original_controller_params_struct.LQRAgnostic_params;
            LQRDiffIntegralAgnostic_params = original_controller_params_struct.LQRDiffIntegralAgnostic_params;
            MPCDiscreteDiffIntegralAgnostic_params = original_controller_params_struct.MPCDiscreteDiffIntegralAgnostic_params;
            
            MPCDiscrete_function_parameters= original_controller_params_struct.MPCDiscrete_function_parameters;
            MPCPseudoDiscrete_function_parameters= original_controller_params_struct.MPCPseudoDiscrete_function_parameters;
            LQR_function_parameters= original_controller_params_struct.LQR_function_parameters;
            LQRDiffIntegral_function_parameters= original_controller_params_struct.LQRDiffIntegral_function_parameters;
            MPCDiscreteAgnostic_function_parameters= original_controller_params_struct.MPCDiscreteAgnostic_function_parameters;
            MPCPseudoDiscreteAgnostic_function_parameters= original_controller_params_struct.MPCPseudoDiscreteAgnostic_function_parameters;
            LQRAgnostic_function_parameters= original_controller_params_struct.LQRAgnostic_function_parameters;
            LQRDiffIntegralAgnostic_function_parameters= original_controller_params_struct.LQRDiffIntegralAgnostic_function_parameters;
            MPCDiscreteDiffIntegralAgnostic_function_parameters= original_controller_params_struct.MPCDiscreteDiffIntegralAgnostic_function_parameters;
            % End of copy-paste

            eval(MPC_name_prefix + "_params = controller_params_struct." +  MPC_name_prefix + "_params;");
            eval(MPC_name_prefix + "_function_parameters= controller_params_struct." +  MPC_name_prefix + "_function_parameters;");
%             

%             plot(reshape(LQRAgnostic_params.K_lqr,1,[]))
%             hold off
            %% Prepare the simulation, etc.
            % Add disturbances
            reg.input_type_Q_grate = InputType.Experimental;
            reg.input_type_T_aI = InputType.Experimental;
            reg.input_type_T_aII = InputType.Experimental;
            reg.controller_type = ControllerType.MPC;
            reg.include_noise = false;
            %% Simulate
            options = simset('SrcWorkspace', 'current');
%             options.TimeOut =4; 
            simout = sim(simulink_model_name, [], options);
            simulation_results = set_simout_to_struct(simout);
            simout_F_st   =  simulation_results.F_st;
            simout_F_O2   =  simulation_results.F_O2;
            simout_F_aI   =  simulation_results.F_aI;
            simout_F_aII  =  simulation_results.F_aII;
            %% Calculate costs
            get_deviation = @(ts, r) squeeze(abs(resample(ts, simout_F_st.time(1):1:simout_F_st.time(end)).data - r)); % Resample and subtract from reference
            steam_error = get_deviation(simout_F_st, lqe_params.y_star(1));
            O2_error = get_deviation(simout_F_O2, lqe_params.y_star(2));
            air_diff_error = get_deviation(simout_F_aI - simout_F_aII, lqe_params.u_star(3) - lqe_params.u_star(1));
%             plot(steam_error)
            overshoot_cost = 1e-3*(norm(steam_error, Inf) + norm(O2_error, Inf) + 1e-7 * norm(air_diff_error, Inf));
            L2_norm_cost = 1e1*((norm(steam_error, 2) + norm(O2_error, 2) + 1e-7 * norm(air_diff_error, 2)) / length(steam_error));
            const_error_cost = (0.01 * abs(sum(steam_error)) + 0.01 * abs(sum(O2_error))) / length(steam_error); % Punish if the system does not converge to reference properly
            sim_cost = overshoot_cost + L2_norm_cost + const_error_cost;
            
            % A bode-plot is used to get an estimate of the errorst caused by noise
            min_w_noise = 10; % Assume the noise starts at a frequency at around 0.1 (Maybe??)
            max_w_disturbance = 0.03; 
            bode_cost_weights = [1,1, 1e-4]; 
            if contains(MPC_name_prefix, "Agnostic")
                [noise_cost,bode_disturbance_cost]= alternative_bode_cost(eval(MPC_name_prefix + "_params"), agnostic_lqe_params, full_estimated_plant, min_w_noise,max_w_disturbance,bode_cost_weights);
            else
                [noise_cost,bode_disturbance_cost] = alternative_bode_cost(eval(MPC_name_prefix + "_params"), lqe_params, full_estimated_plant, min_w_noise,max_w_disturbance,bode_cost_weights);
            end
            if 1e15*sim_cost <= 30
                if contains(MPC_name_prefix, "Agnostic")
                    robustness_cost =robustness_based_costs(eval(MPC_name_prefix+"_params"), agnostic_lqe_params, MPC_name_prefix);
                else
                    robustness_cost =robustness_based_costs(eval(MPC_name_prefix+"_params"), lqe_params, MPC_name_prefix);
                end
            else
                noise_cost = 100;
                robustness_cost = 100; % Save some computations?
            end
% %             cost = sim_cost + (bode_cost / (2));
          cost = 1e-12*noise_cost + 1e-3*robustness_cost+ 1e5*sim_cost;
%         cost = bode_cost/2 + robustness_cost*1e7; % Robustness cost becomes 0 if the plant is stame at the probing points
%         cost = bode_cost/2;
        catch ME
            ME
            cost = 1e20; % A big punishment if the system is unstable
        end
        
%         sprintf("Cost: %i <- %s", cost, sprintf("%.12f, ", parameters))

%             figure(3)
%             hold on 
% %             plot(air_diff_error)
%                (plot(simout_F_O2- lqe_params.y_star(2)))
%     the_big_root_locus_robustness_analysis( agnostic_lqe_params, LQRDiffIntegralAgnostic_params, "LQRDiffIntegralAgnostic_params")
    end

    

end
