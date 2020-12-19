% run prepare_furnce

% run create_MPC


% reg.controller_type = ControllerType.MPC; 

% reg.input_type_Q_grate = InputType.Experimental;
% reg.input_type_T_aI = InputType.Experimental;
% reg.input_type_T_aII = InputType.Experimental;
% reg.controller_type = ControllerType.MPC; 


quick_and_easy_mpc = mpc( ...
mpc_params.MPC_plant, ...
mpc_params.MPC_plant.Ts, ...
mpc_params.mpc_function_params.N_prediction_steps ...
); 