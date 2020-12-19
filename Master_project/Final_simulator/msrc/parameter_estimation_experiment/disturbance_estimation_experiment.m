
run prepare_furnce.m
%% Make a real exper<iment 
% Only constant
old_reg = reg; 
reg = set_single_step_in_reg(old_reg, "F_aII");
reg.input_type_F_aII = InputType.Constant; % Constant step response
tic 
sim Furnace
toc 
constant_response = {simout_F_st, simout_F_O2, simout_HHV}; 
% all_affecting_parameters= ["F_aII" "F_w_in"  "F_aI"  "v_grate","T_aI", "T_aII"]; %Internal ordering
original_controller_suffixes  = ["F_aII" "F_w_in"  "F_aI"  "v_grate","T_aI", "T_aII"]; %Internal ordering
% original_controller_suffixes = ["F_aII" "F_w_in"  "F_aI"  "v_grate"]; %Internal ordering
disturbance_measurement_indecies = 3:5; 
controller_suffixes  = original_controller_suffixes ;
step_responses = cell(3,length(controller_suffixes)); 
%%
for i = 1:length(controller_suffixes )
%     reg = set_single_step_in_reg(old_reg, controller_suffixes(i)); 
    
    reg = old_reg; 
    
    % ..............
    for current_suffix= controller_suffixes
        eval(sprintf("reg.input_type_%s = InputType.Constant;", current_suffix));
    end
    step_name = controller_suffixes(i);
    eval(sprintf("reg.input_type_%s = InputType.Step;", step_name));
    
    %
    disp("Simulate: "+ step_name); 
    tic
    sim Furnace
    simulation_duration = toc
    current_step_responses = {simout_F_st, simout_F_O2, simout_HHV}; 
    step_times{i} = reg.stepTime; 
    for j = 1:3
        step_responses{j,i} = current_step_responses{j}; 
    end
end
%% Handle the three Q_grate parameters
controller_suffixes = original_controller_suffixes; %Make sure the suffixes are the same
for current_suffix= controller_suffixes % There are 3 inputs in Q_grate, so it has to be treated spechially
    eval(sprintf("reg.input_type_%s = InputType.Constant;", current_suffix));
end
reg.input_type_Q_grate = InputType.Step; 
for Q_no = 1:3
    
    i = i+1; 
    reg.const_Q_grate_post_step = reg.const_Q_grate;
    reg.const_Q_grate_post_step(Q_no) = reg.const_Q_grate_post_step(Q_no)*1.01; 
    Q_no
    tic
    sim Furnace
    simulation_duration = toc
    current_step_responses = {simout_F_st, simout_F_O2, simout_HHV}; 
    step_times{i} = reg.stepTime; 
    for j = 1:3
        step_responses{j,i} = current_step_responses{j}; 
    end
end
controller_suffixes = [controller_suffixes , "Q_grate_1","Q_grate_2","Q_grate_3"]; 

"Sims done"
"Identification experiment done"

%% Quick verification 
for input_no = 1:length(controller_suffixes)
    for output_no = 1:3
        subplot(3,length(controller_suffixes),input_no + (output_no -1)*length(controller_suffixes))
        plot(step_responses{output_no, input_no} - mean(step_responses{output_no, input_no}))
        title(sprintf("Mean = %.2d",mean(step_responses{output_no, input_no})))
        xlabel( controller_suffixes(input_no)); 
    end
end
%% Add parameter estimation here
u_star = zeros([length(controller_suffixes),1]); 
u_post_step = zeros([length(controller_suffixes),1]); 
for i = 1:(length(controller_suffixes)-3)
   u_star(i) = eval(sprintf("reg.const_%s",controller_suffixes(i)));
   u_post_step (i) = eval(sprintf("reg.const_%s_post_step",controller_suffixes(i)));
end

u_star(end-2:end) = reg.const_Q_grate';
u_post_step(end-2:end) =  u_star(end-2:end)*1.01; 
%% Save everything, so somebody else can do system ID
step_amplitudes = num2cell(u_post_step - u_star);
step_time = step_times{1}; % They must all be the same
save("outputs/expanded_disturbance_estimation_experiment_result.mat", "u_star", "constant_response", "step_responses", "step_amplitudes", "step_time", "controller_suffixes", "disturbance_measurement_indecies")

