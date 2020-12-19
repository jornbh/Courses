clear all %Just to be sure
%% Define the controller-types
addpath(genpath(pwd));
PID_controller_names = ["AB", "Cascade"]; 
PID_controller_types = [ControllerType.AB, ControllerType.Cascade];
optimal_controller_types = MPCType(1:9); 
optimal_controller_types = optimal_controller_types(3:end) %Delete this (Removes the bad MPCs) 
optimal_controller_names_string = sprintf("%s,", optimal_controller_types);
optimal_controller_names = split(optimal_controller_names_string , ",")'; 
optimal_controller_names = optimal_controller_names(1:end-1); % The last sub-string is empty

optimal_controller_names = optimal_controller_names(5) % Delete these to run more kinds of optimal controllers
optimal_controller_types = optimal_controller_types(5) % Delete these to run more kinds of optimal controllers
%% Uncomment all required controllers
system_name = "sandbox"; 
if system_name == "Furnace"
    block_root = 'Furnace/Controller/MPC_subsystem/LQG_controller/Controller_battery/'; % Furnace root 
else
    block_root =  'sandbox/LQG_controller/MPC_subsystem/LQG_controller/Controller_battery/';
end
load_system(system_name); % Allows for doing things with a simulink model

for comment_block = MPCType(1:9)
    set_param(block_root + sprintf("%s",comment_block) ,'commented','on')
end


for uncomment_block = optimal_controller_names
    set_param(block_root + uncomment_block ,'commented','off')
end

%%
run create_MPC; 
if system_name  == "Furnace"
    run prepare_furnce
else
    load full_estimated_plant % Simplified plant used by the sandbox
    
end
FurnaceParameters

% run create_MPC
%% Primary disturbance type
% reg.input_type_Q_grate = InputType.Experimental;
% reg.input_type_T_aI = InputType.Experimental;
% reg.input_type_T_aII = InputType.Experimental;
% disturbance_type_str = "Experimental"

%% Alternative disturbance

reg.input_type_Q_grate = InputType.Constant;
reg.input_type_T_aI = InputType.Constant;
reg.input_type_T_aII = InputType.Constant;

reg.input_type_Q_grate  = InputType.Step; 
reg.const_Q_grate_post_step =  reg.const_Q_grate .* [1.5,1.5,1.5]; 
disturbance_type_str = "Step_Q_all"; 

%% Sim PIDs 
% set_param("Furnace/Controller/MPC_subsystem/LQG_controller/x_hat_selector", "sw","0")    
% set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch', "sw","0")    
% set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch1', "sw","0")    
for i = 1:length(PID_controller_names)
    reg.controller_type = PID_controller_types(i);
    fprintf("Start sim %s\n", PID_controller_names(i))
    tic
    current_simout = sim(system_name);
    toc
    eval(sprintf("%s_results = set_simout_to_struct(current_simout);", PID_controller_names(i)));
    eval(sprintf("%s_results.u_star = [reg.op_F_aII, reg.op_F_w_in, reg.op_F_aI,reg.op_v_grate] ;", PID_controller_names(i)));
    eval(sprintf("%s_results.y_star = [reg.op_F_st, reg.op_F_O2];", PID_controller_names(i)));
    eval(sprintf("%s_results.air_ratio_star = [reg.op_F_aI -  reg.op_F_aII];", PID_controller_names(i)));

end


%% Sim Optimal controllers
% set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch', "sw","1")    
% set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch1', "sw","1")    
% set_param("Furnace/Controller/MPC_subsystem/LQG_controller/x_hat_selector", "sw","1");    
reg.controller_type = ControllerType.MPC;
for i =1:length(optimal_controller_names)
    try
    MPC_type = optimal_controller_types(i);
    fprintf("Start sim %s\n", optimal_controller_names(i));
    tic 
    current_simout = sim(system_name);
    toc 
    eval(sprintf("%s_results = set_simout_to_struct(current_simout);", optimal_controller_names(i)));
    eval(sprintf("%s_results.u_star = lqe_params.u_star;", optimal_controller_names(i)));
    eval(sprintf("%s_results.y_star = lqe_params.y_star;", optimal_controller_names(i)));
    eval(sprintf("%s_results.air_ratio_star = [lqe_params.u_star(3) - lqe_params.u_star(1)];", optimal_controller_names(i)));
    catch ME 
        ME 
    end
end


%% Plot the results
%#TODO Unncomment this
% plot_start_time = Cascade_results.F_aI.time(1)+3e2 ; 
% plot_end_time = Cascade_results.F_aI.time(end);
% line_width = 1; 
% 
% % Single plots 
% % Optimal controll against PID 
% % All together 
% single_names = [PID_controller_names, optimal_controller_names]; 
% 
% 
% for plot_ind =  1:(2*length(single_names)+2)% optimal_controller_names(1:end)%[PID_controller_names, optimal_controller_names]
% %     plotable_names = [single_name]
% 
% 
%     if plot_ind <= length(single_names)
%         plotable_names = single_names(plot_ind); 
%     elseif plot_ind <= length(single_names)*2
%         plotable_names = [PID_controller_names, single_names(plot_ind - length(single_names))]; 
%     elseif plot_ind == length(single_names)*2 +1 
%         plotable_names = PID_controller_names
%     else
%         plotable_names  = [PID_controller_names, optimal_controller_names]; 
%     end
% %     plotable_names = [PID_controller_names, single_name]
%     line_styles = jet(length(plotable_names));
%     line_styles  = [line_styles, 0.7*ones([size(line_styles,1),1])]; % Add some transparancy
%     for i =  10:11 % Output, Input and air-diff figure
%         try
%             close(i) % Close the figure if it is open
%         catch
%         end
%         figure(i)
%     end
%     for i = 1:length(plotable_names)
%         % Plot Outputs
%         figure(10)
%         result = eval(sprintf("%s_results", plotable_names(i)));
%         lines = [result.F_st- result.y_star(1),
%                 result.F_O2 - result.y_star(2), 
%                 result.F_aI - result.F_aII - result.air_ratio_star	]; 
%         for j = 1:3
%             subplot(3,1,j)
%             hold on
%             plot(lines(j), "-", "Color", line_styles(i,:),"LineWidth", line_width)
%             hold off
%             xlim([plot_start_time, plot_end_time]); 
%         end
% 
%         % Plot inputs
%         figure(11)
%         lines = [result.F_aII, result.F_w, result.F_aI, result.v_grate]; 
%         for j =1:4
%             subplot(2,2,j)
%             hold on 
%             plot(lines(j) - result.u_star(j), "-", "Color", line_styles(i,:),"LineWidth", line_width)
%             hold off 
%             xlim([plot_start_time, plot_end_time]); 
%         end
%     end
%     figure(10)
%     subplot(3,1,1)
%     legend(strrep(plotable_names,"_"," "))
%     output_names = ["F_{st}", "F_{O2}", "F_{aI} - F_{aII}"]; 
%     output_units = ["[kg/s]", "[kg/s]", "[kg/s]"]; 
%     for i = 1:3
%         subplot(3,1,i)
%         title(output_names(i))
%         xlabel("time [s]")
%         ylabel(output_units(i))
%     end
%     sgtitle("Outputs")
%     figure(11)
%     subplot(2,2,1)
%     legend(strrep(plotable_names,"_"," "))
%     input_names = ["F_{aII}", "F_{w,in}", "F_{aI}", "v_{grate}"];
%     input_units = ["[kg/s]", "[kg/s]", "[kg/s]", "[m/s]"]; 
%     for i = 1:4
%         subplot(2,2,i)
%         title(input_names(i));
%         xlabel("time [s]")
%         ylabel(input_units(i))
%     end
%     sgtitle("Inputs")
%     saveas(figure(10),"./JornDir/Fig_dump/"+"outputs_" + sprintf("%s", [plotable_names]) + disturbance_type_str , "epsc")
%     saveas(figure(11),"./JornDir/Fig_dump/"+"inputs_" + sprintf("%s", [plotable_names]) + disturbance_type_str, "epsc")
% end