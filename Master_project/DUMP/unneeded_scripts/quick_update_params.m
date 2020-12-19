controller_name = "LQRDiffIntegralAgnostic"; 

MPC_name_suffixes = [controller_name]
pre_prepared_weights = load("committed_best_"+controller_name+"_params"); 
%% Change the weights around 
pre_prepared_weights.Rd = pre_prepared_weights.Rd *1e3; 
pre_prepared_weights.R = pre_prepared_weights.R *1e2; 

%% Update controller
[controller_params_struct] = create_MPC(...
                pre_prepared_weights, ...
                original_lqe_params , ... % Optional params
                original_agnostic_lqe_params,...
                MPC_name_suffixes);

fields = fieldnames(controller_params_struct)

for i = 1:length(fields)
    eval( fields{i} +"="+ "controller_params_struct."+fields{i})
end

sim("Furnace")