%% Load system 
if ~exist("start_index","var")
    start_index = 5;
end
addpath(genpath(pwd));
simulink_model_name ="sandbox";
load_system(simulink_model_name)
%% Itterates through all controllers that can controll the plant and does something
reg.controller_type = ControllerType.MPC; 
if simulink_model_name == "sandbox"
block_root = 'sandbox/LQG_controller/MPC_subsystem/LQG_controller/Controller_battery/'; % Sandbox root 
else
block_root = 'Furnace/Controller/MPC_subsystem/LQG_controller/Controller_battery/'; % Furnace root 
end


controller_types = MPCType(1:9); 
suffixes_string = sprintf("%s,", controller_types);
suffixes = split(suffixes_string, ",")'; 
suffixes = suffixes(1:end-1); % The last sub-string is empty
global use_disturbance_measurements 
use_disturbance_measurements = false; 

for tuned_index =  ([start_index:length(suffixes), 1:(start_index-1)]) % Tune the easy types first [start_index]
    tuned_index
    set_param(simulink_model_name ,'FastRestart','off')

    for cleared_suffix= suffixes
        set_param(block_root + cleared_suffix ,'commented','on')
    end
    
    set_param(block_root + suffixes(tuned_index) ,'commented','off')
    MPC_type = controller_types(tuned_index);
    
    % Set the MPC function params
    create_MPC
%     eval(sprintf("%s_function_parameters = mpc_params.mpc_function_params;",suffixes(tuned_index)));
%     
%     % Set the remainding parameters
%     eval(sprintf("%s_mpc_params = mpc_params;", suffixes(tuned_index)))
%     
    global MPC_name_prefix
    MPC_name_prefix = suffixes(tuned_index);
    % Simulation/optimizing part

    run MPC_battery_search_for_parameters; % Find the best set of parameters      eval(sprintf("best_%s_params = best_params;",suffixes(tuned_index)))
    eval(sprintf("save('outputs/best_%s_params','best_params', 'R','Rd', 'Qy','Qy_diff', 'R_lqe', 'Q_lqe', 'Q_lqe_agnostic', 'Qy_extended')",suffixes(tuned_index)))
    figure(8); 
    hold on 
    plot(tuned_index,1, "x")
    hold off
    title("Tuned controllers")
%     try
%         sim("Furnace")
%         figure(9)
%         subplot(2,1,1)
%         plot(simout_F_st)
%         subplot(2,1,2)
%         plot(simout_F_O2)
%     catch ME
%         ME
%     end
        

end
hold off
legend(suffixes)