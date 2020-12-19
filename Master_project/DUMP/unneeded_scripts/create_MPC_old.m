addpath(genpath(pwd));



%% ERA system identification 


load('disturbance_estimation_experiment_result.mat')
[full_estimated_plant_d,~, tau_anti_alias_filter] = Step_method_main( constant_response, step_responses, step_amplitudes, step_time, 15,u_star, 7); 
full_estimated_plant = d2c(full_estimated_plant_d);
approxomated_system = full_estimated_plant_d * eye(size(full_estimated_plant_d.B,2),4);

u_star_expanded = u_star;
u_star = u_star(1:4); % Only the controllable inputs
G_estimated= full_estimated_plant_d.B(:,5:end); % Only the disturbances
G_lqe = G_estimated; 
G_lqe_measurements = G_estimated;% zeros(size(d2c(full_estimated_plant).B(:,5:end)));


%%ERA on the system without assumptions about the disturbances 
input_step_responses = {step_responses{:,1:4}};
input_step_responses  = reshape(input_step_responses, [3,4]);
input_step_amplitudes = {step_amplitudes{1:4}};
[agnostic_approxomated_system,~, agnostic_tau_anti_alias_filter] = Step_method_main( constant_response, input_step_responses, step_amplitudes, step_time, 12,u_star, 7); 

% => approxomated_system ,agnostic_approxomated_system
 
%% Overwrite with the autotuned results instead 
%!Weights
discrete_MPC_weights = load('committed_best_Discrete_MPC_params.mat');
pseudo_discrete_MPC_weights = load('secret_school_Pseudo_contineous_MPClast_mpc_params.mat');
lqr_weights = load("committed_best_LQR_params.mat");
diff_integrated_weights = load('committed_best_LQR_integrated_params.mat');

% Agnostic weights
discrete_MPC_agnostic_weights =  load('committed_best_Discrete_MPC_params.mat'); %TODO: Fix this one
lqr_agnostic_weights = load("committed_best_LQR_params.mat");
integrated_diff_lqr_agnostic_weights = load('committed_best_LQR_integrated_agnostic_params.mat');
pseudo_discrete_agnostic_MPC_weights =  load('secret_school_Pseudo_contineous_MPClast_mpc_params.mat');
MPC_dt = 20; 
diff_integrated_weights.R_lqe = diff_integrated_weights.R_lqe *1e-3; % Less aggressive estimator makes convergence easier. 
%% Create an integral MPC
LQG_plant = d2c( approxomated_system); %The lqg can only work on contineous systems
controlled_plant = extract_controlled_plant(LQG_plant, [1,2], "all"); 

lqe_weights = discrete_MPC_weights; %diff_integrated_weights;
Q_lqe_cut = lqe_weights.Q_lqe(disturbance_measurement_indecies,disturbance_measurement_indecies);
lqe_params = make_lqe(LQG_plant, Q_lqe_cut, lqe_weights.R_lqe, G_lqe, tau_anti_alias_filter); 

lqe_params.G_lqe = G_lqe; 
lqe_params.G_lqe_measurements = G_lqe_measurements;
lqe_params.integrator_direct_measurement_belief = 1; 
lqe_params.disturbance_measurement_indecies = disturbance_measurement_indecies;

lqr_params = make_MPC_lqr_style(controlled_plant, ...
                    lqr_weights .Qy, ...
                    lqr_weights .Qy_diff, ...
                    lqr_weights .R, ...
                    lqr_weights .Rd, 20,MPC_dt); 
                
% diff_integrated_lqr_params = make_MPC_lqr_style(controlled_plant, ...
%                     diff_integrated_weights.Qy, ...
%                     diff_integrated_weights.Qy_diff, ...
%                     diff_integrated_weights.R, ...
%                     diff_integrated_weights.Rd, 20,20); 

%% Make a discrete MPC 
discrete_mpc_params = make_MPC_lqr_style(controlled_plant, ...
                    discrete_MPC_weights.Qy, ...
                    discrete_MPC_weights.Qy_diff, ...
                    discrete_MPC_weights.R, ...
                    discrete_MPC_weights.Rd, 20,MPC_dt); 
discrete_MPC_function_parameters = discrete_mpc_params.mpc_function_params; % Used in simulink

%% Make pseudo-discrete MPC
pseudo_discrete_mpc_params = make_MPC_lqr_style(controlled_plant, ...
                    pseudo_discrete_MPC_weights.Qy, ...
                    pseudo_discrete_MPC_weights.Qy_diff, ...
                    pseudo_discrete_MPC_weights.R, ...
                    pseudo_discrete_MPC_weights.Rd, 20,MPC_dt); 
pseudo_discrete_MPC_function_parameters = pseudo_discrete_mpc_params.mpc_function_params; % Used in simulink


%% Make agnostic lqe 

agnostic_lqe_params = make_lqe(d2c(agnostic_approxomated_system ), integrated_diff_lqr_agnostic_weights.Q_lqe_agnostic, integrated_diff_lqr_agnostic_weights.R_lqe,eye(size(agnostic_approxomated_system.A,1)), agnostic_tau_anti_alias_filter); 
% mpc_params = Main_MPC_diff_style(approxomated_system, Qy,Qy_diff,R, Rd, Q_lqe, R_lqe,5,17, [1,2]); 
%% System with integral punishment for aI - aII

diff_air_ss = ( tf(1,[1])); % No states will be added by this
extended_approxomated_system = append(controlled_plant,diff_air_ss)*[eye(4); [-1,0,1,0]];
diff_integrated_lqr_params = make_MPC_lqr_style(extended_approxomated_system, ...
    diff_integrated_weights.Qy_extended,...,
    blkdiag(diff_integrated_weights.Qy_diff,0),...    
    diff_integrated_weights.R,...
    diff_integrated_weights.Rd, ...
    20,1); 


lqe_params.u_star = u_star; 
for i = 1:3 
    lqe_params.y_star(i) = constant_response{i}.data(end); 
end 
lqe_params.v_star = u_star_expanded(5:end); % The constant disturbances to the plant

mpc_function_type = discrete_mpc_params.mpc_function_parameter_type;
discrete_mpc_params.mpc_ref = [0;0]; 


%% Extra controllers (Don't assume where the disturbances come from) 
%!AC
    agnostic_plant = eye(2,3)*d2c(agnostic_approxomated_system);
    agnostic_diff_plant = append(agnostic_plant, [-1,0,1,0]) * kron([1;1], eye(4));
                   

    discrete_agnostic_mpc_params = make_MPC_lqr_style(agnostic_plant,discrete_MPC_agnostic_weights.Qy,...
                                            discrete_MPC_agnostic_weights.Qy_diff,...
                                            discrete_MPC_agnostic_weights.R,...
                                            discrete_MPC_agnostic_weights.Rd, ...
                                            20,MPC_dt );


                                        
    agnostic_pseudo_discrete_mpc_params = make_MPC_lqr_style(agnostic_plant,pseudo_discrete_agnostic_MPC_weights.Qy,...
                                            pseudo_discrete_agnostic_MPC_weights.Qy_diff,...
                                            pseudo_discrete_agnostic_MPC_weights.R,...
                                            pseudo_discrete_agnostic_MPC_weights.Rd, ...
                                            20,MPC_dt);

                   
    agnostic_lqr_params = make_MPC_lqr_style(agnostic_plant,pseudo_discrete_agnostic_MPC_weights.Qy,...
                                            pseudo_discrete_agnostic_MPC_weights.Qy_diff,...
                                            pseudo_discrete_agnostic_MPC_weights.R,...
                                            pseudo_discrete_agnostic_MPC_weights.Rd, ...
                                            1,0.1);
                     
                                        
                                        
    diff_agnostic_lqr_params = make_MPC_lqr_style(agnostic_diff_plant,integrated_diff_lqr_agnostic_weights.Qy_extended,...
                                            blkdiag(integrated_diff_lqr_agnostic_weights.Qy_diff,0),...
                                            integrated_diff_lqr_agnostic_weights.R,...
                                            integrated_diff_lqr_agnostic_weights.Rd, ...
                                            1,0.1);
                                                             
                                        
agnostic_MPC_function_parameters = discrete_agnostic_mpc_params.mpc_function_params;
agnostic_pseudo_discrete_MPC_function_parameters = agnostic_pseudo_discrete_mpc_params.mpc_function_params; 

%% The estimations wer based on discrete samples, so some anti-aliasing is needed
anti_aliasing_filter = ss( tf(1,[tau_anti_alias_filter,1]) * eye(size(full_estimated_plant_d,1)));
anti_aliasing_filter_agnostic = ss( tf(1,[agnostic_tau_anti_alias_filter,1]) * eye(size(full_estimated_plant_d,1)));


% MPC_function_parameters = mpc_params.mpc_function_params; 
save("last_mpc_params.mat", "lqe_params",...
    "diff_mpc_params", "diff_agnostic_mpc_params","agnostic_lqe_params", ...
    "MPC_function_parameters", "anti_aliasing_filter", "anti_aliasing_filter_agnostic", "discrete_mpc_params", "discrete_MPC_function_parameters" );

