function [controller_params_struct, lqe_params_struct] = create_MPC(...
                pre_prepared_weights, ...
                original_lqe_params , ... % Optional params
                original_agnostic_lqe_params,...
                MPC_name_suffixes)


%% ERA system identification
if ~exist("original_agnostic_lqe_params", "var") % Normal system identification
    load('disturbance_estimation_experiment_result.mat')
    [full_estimated_plant_d,~, tau_anti_aliasing_filter] = Step_method_main( constant_response, step_responses, step_amplitudes, step_time, 15,u_star, 7); 
    full_estimated_plant = d2c(full_estimated_plant_d);
    approxomated_system = full_estimated_plant_d * eye(size(full_estimated_plant_d.B,2),4);

    u_star_expanded = u_star; % The loaded u^* is both inputs and disturbances
    u_star = u_star_expanded(1:4); % Only the controllable inputs
    v_star = u_star_expanded(5:end);
    G_estimated= full_estimated_plant_d.B(:,5:end); % Only the disturbances
    G_lqe = G_estimated; 
    G_lqe_measurements = G_estimated;% zeros(size(d2c(full_estimated_plant).B(:,5:end)));


    %%ERA on the system without assumptions about the disturbances 
    input_step_responses = {step_responses{:,1:4}};
    input_step_responses  = reshape(input_step_responses, [3,4]);
    input_step_amplitudes = {step_amplitudes{1:4}};
    [agnostic_approxomated_system,~, tau_agnostic_anti_aliasing_filter] = Step_method_main( constant_response, input_step_responses, step_amplitudes, step_time, 12,u_star, 7); 
    
    y_star = zeros([3,1]); 
    for i = 1:3 
    y_star(i) = constant_response{i}.data(end); 
    end 
else % If an old version is provided, then sys-ID can be skipped
    u_star = original_lqe_params.u_star; 
    v_star =original_lqe_params.v_star;
    y_star = original_lqe_params.y_star; 
    G_lqe = original_lqe_params.G_lqe; 
    G_lqe_measurements = original_lqe_params.G_lqe_measurements;
    disturbance_measurement_indecies = original_lqe_params.disturbance_measurement_indecies; 
    approxomated_system = c2d(original_lqe_params.observed_plant,0.1);
    agnostic_approxomated_system = c2d(original_agnostic_lqe_params.observed_plant,0.1);
    tau_anti_aliasing_filter = original_lqe_params.tau_anti_aliasing_filter;
    tau_agnostic_anti_aliasing_filter= original_agnostic_lqe_params.tau_anti_aliasing_filter;
    
end
% => approxomated_system ,agnostic_approxomated_system
MPC_dt = 20; 


controlled_plant = eye(2,3)*d2c( approxomated_system);
controlled_diff_plant = append(controlled_plant, [-1,0,1,0]) * kron([1;1],eye(4));
controlled_agnostic_plant = eye(2,3)*d2c( agnostic_approxomated_system);
controlled_diff_agnostic_plant = append(controlled_agnostic_plant, [-1,0,1,0]) * kron([1;1],eye(4));
%% Make LQR
do_save = ~exist("MPC_name_suffixes", "var"); % Do not save if only some values are assigned
if ~exist("MPC_name_suffixes", "var") % Make for all controller types
    MPC_name_suffixes = split(sprintf("%s,", MPCType(1:9)),","); %Controller types
    MPC_name_suffixes = MPC_name_suffixes(1:end-1)'; % The last one is bad (Also wrong direction for for-loops)
end

for MPC_name = MPC_name_suffixes 
    
    if ~exist("pre_prepared_weights", "var")
        weights = load("committed_best_"+MPC_name+"_params.mat"); 
    else 
        weights = pre_prepared_weights; 
    end


   % Find the correct weights 
   if contains(MPC_name,"Diff")
    Qy = weights.Qy_extended; 
    Qy_diff = blkdiag(weights.Qy_diff,0);
   else
       Qy = weights.Qy; 
       Qy_diff = weights.Qy_diff; 
   end
   R = weights.R;
   Rd = weights.Rd; 

    % Set sampling time
    N_steps = 1; 
    controller_dt = 0.1; % Contineous controllers have a faster sampling time
    if contains(MPC_name, "MPC")
       controller_dt = 20; 
       N_steps = 20; 
    end
   
   % Find the right plant
   diff_code ="";
   agnostic_code =""; 
   if contains(MPC_name, "Diff")
       diff_code = "diff_"; 
   end
   if contains(MPC_name, "Agnostic")
       agnostic_code = "agnostic_"; 
   end

   plant = eval( "controlled_"+ diff_code+agnostic_code + "plant"); 
   
   controller_params = make_MPC_lqr_style(plant, Qy, Qy_diff, R,Rd, N_steps, controller_dt); 
%    controller.params.input_filter = 
   % Assign all Simulink variables
   eval(MPC_name + "_params = controller_params;");
   eval(MPC_name + "_function_parameters = controller_params.mpc_function_params;");
   eval(MPC_name + "_function_type  = controller_params.mpc_function_parameter_type;");
end

%% Make lqe
if ~exist("pre_prepared_weights", "var")
    lqe_weights =load("committed_best_LQRDiffIntegral_params.mat");
    lqe_agnostic_weights = load("committed_best_LQRDiffIntegralAgnostic_params.mat");
else
    lqe_agnostic_weights = pre_prepared_weights;
    lqe_weights = pre_prepared_weights;
end
Q_lqe_disturbances = lqe_weights.Q_lqe(disturbance_measurement_indecies,disturbance_measurement_indecies); 
lqe_params = make_lqe(d2c(approxomated_system), Q_lqe_disturbances, lqe_weights.R_lqe, G_lqe, tau_anti_aliasing_filter); 
agnostic_lqe_params = make_lqe( d2c(agnostic_approxomated_system), lqe_agnostic_weights.Q_lqe_agnostic, lqe_agnostic_weights.R_lqe,eye(size(agnostic_approxomated_system.A,1)), tau_agnostic_anti_aliasing_filter);

% Extra assignments
lqe_params.G_lqe = G_lqe; 
lqe_params.G_lqe_measurements = G_lqe_measurements; 
lqe_params.v_star = v_star;
lqe_params.u_star = u_star;
lqe_params.integrator_direct_measurement_belief =1; % How much the integrated state is based on direct measurements
lqe_params.disturbance_measurement_indecies = disturbance_measurement_indecies; 
agnostic_lqe_params.u_star = u_star;
agnostic_lqe_params.integrator_direct_measurement_belief =1; 
lqe_params.y_star = y_star; 
agnostic_lqe_params.y_star = y_star; 



%% Save all
anti_aliasing_filter = ss( tf(1,[tau_anti_aliasing_filter,1]) * eye(size(approxomated_system,1)));
anti_aliasing_filter_agnostic = ss( tf(1,[tau_agnostic_anti_aliasing_filter,1]) * eye(size(approxomated_system,1)));

if do_save
    controller_names = cellstr(MPC_name_suffixes+ "_params");
    function_param_names = cellstr(MPC_name_suffixes+ "_function_parameters");
    save("outputs/last_mpc_params.mat", "lqe_params", "agnostic_lqe_params",...
        controller_names{:}, ...
        function_param_names{:}, ...
        "anti_aliasing_filter", "anti_aliasing_filter_agnostic");
end

%% Lump all controller parameters and estimator parameters into one big thing
for suffix_name = MPC_name_suffixes
    eval("controller_params_struct."+ suffix_name +"_params = " + suffix_name +"_params;")
    eval("controller_params_struct."+ suffix_name +"_function_parameters = " + suffix_name +"_function_parameters;")
end

lqe_params_struct.lqe_params = lqe_params;
lqe_params_struct.agnostic_lqe_params = agnostic_lqe_params; 
% figure(3)
% hold on 
% %             plot(air_diff_error)
% plot(reshape(LQRAgnostic_params.K_lqr,1,[]))
% hold off
% end % Function