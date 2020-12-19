%% Set up MPC and make simple simulation
function mpc_params = Integral_MPC_main(mpc_cost_function_name,...
    controlled_plant_c,...
    Qy, ...
    Qy_diff, ...
    R, ...
    Rd, ...
    N_prediction_steps, ...
    MPC_time_step)
    %% Parameters 
    
    
    
    filter_time_constant =  MPC_time_step; %Filter to ensure properness (Just chose something short
    
    
%     [controled_plant , observed_plant,                              ] = make_contineous_plants(estimated_plant_discrete,controlled_output_indecies,controlled_input_indecies);
    [filtered_plant  , input_filter                                 ] = add_input_filter_to_plant(controlled_plant_c, filter_time_constant);
    [integrator_plant, reference_B                                  ] = add_integrator_to_plant(filtered_plant);
    [MPC_plant                                                      ] = c2d(integrator_plant, MPC_time_step); % Normal function
    [M               , CONV                                         ] = make_MPC_response(MPC_plant , N_prediction_steps);
    [H_direct               ,  F_y_ref_direct, F_du_direct, F_x0_direct, K_lqr, K_lqr_d] = eval(mpc_cost_function_name +"(MPC_plant , N_prediction_steps, M, CONV, Qy, Qy_diff, R, Rd)");
    [Ac              , b0      , x0_corr, H_slack                     ] = make_MPC_constraints(MPC_plant, M , CONV, N_prediction_steps);
    
    H = blkdiag(H_direct, H_slack);

    F_y_ref = [F_y_ref_direct  ; zeros([size(H_slack,1),size(F_y_ref_direct ,2 )])];   
    F_du    = [F_du_direct     ; zeros([size(H_slack,1),size(F_du_direct    ,2 )])];
    F_x0    = [F_x0_direct     ; zeros([size(H_slack,1),size(F_x0_direct    ,2 )])];

    % A pre-precessed version of the hessian cost H (speeds up MPC)
    L = chol(H, 'lower');
    Linv = L \ eye(size(H, 1));
    % should_simulate_MPC = true
    % Simulate if the right parameter is set
    if exist("should_simulate_MPC", "var") && should_simulate_MPC  == true
        physical_ref = [-2;1];
        simulate_MPC(MPC_plant, physical_ref, H, F_y_ref, F_du, F_x0, Ac,b0, reference_B*MPC_time_step);
    end
    
    %% Set simulink parameters
%     [L_lqe] = make_state_estimator(observed_plant,Q_lqe, R_lqe); 
    [MPC_parameters_type, mpc_function_params] = assign_MPC_parameters_to_simulink(b0, Linv, Ac, N_prediction_steps, F_y_ref, F_du, F_x0, size(MPC_plant.B,2));
    
    %% Assign variables to the output 
%     mpc_params.L_lqe = L_lqe;
    mpc_params.Linv = Linv;
    mpc_params.F_y_ref = F_y_ref;
    mpc_params.F_x0 = F_x0;
    mpc_params.F_du = F_du;
    mpc_params.K_lqr = K_lqr;
    mpc_params.reference_B = reference_B;
    mpc_params.MPC_plant = MPC_plant;
    mpc_params.Ac= Ac;
    mpc_params.b0=b0;
    mpc_params.x0_corr=x0_corr;
    mpc_params.mpc_function_params = mpc_function_params;
    mpc_params.mpc_function_parameter_type  = MPC_parameters_type; 
    mpc_params.controlled_plant = controlled_plant_c;
    mpc_params.input_filter = input_filter; 
    mpc_params.N_refs = size(controlled_plant_c.C,1);
    mpc_params.N_inputs = size(controlled_plant_c.B,2); 
    mpc_params.N_prediction_steps = N_prediction_steps; 
    mpc_params.iA = false(size(mpc_params.b0)); 
    mpc_params.K_lqr_d = K_lqr_d; 
    
    