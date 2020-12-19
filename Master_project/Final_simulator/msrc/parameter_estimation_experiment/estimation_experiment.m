

%% Make a real experiment 
% Only constant
old_reg = reg; 
reg = set_single_step_in_reg(old_reg, "F_aII");
reg.input_type_F_aII = InputType.Constant; % Constant step response
tic 
sim Furnace
toc 
constant_response = {simout_F_st, simout_F_O2, simout_HHV}; 
controller_suffixes = ["F_aII" "F_w_in"  "F_aI"  "v_grate"]; %Internal ordering
step_responses = cell(3,4); 
for i = 1:4 
    reg = set_single_step_in_reg(old_reg, controller_suffixes(i)); 
    tic
    sim Furnace
    simulation_duration = toc
    current_step_responses = {simout_F_st, simout_F_O2, simout_HHV}; 
    step_times{i} = reg.stepTime; 
    for j = 1:3
        step_responses{j,i} = current_step_responses{j}; 
    end
end
"Sims done"
"Identification experiment done"

%% Quick verification 
for input_no = 1:4
    for output_no = 1:3
        subplot(3,4,input_no + (output_no -1)*4)
        plot(step_responses{output_no, input_no} - mean(step_responses{output_no, input_no}))
        title(sprintf("Mean = %.2d",mean(step_responses{output_no, input_no})))
    end
end
%% Add parameter estimation here
u_star = zeros([4,1]); 
u_post_step = zeros([4,1]); 
for i = 1:length(controller_suffixes)
   u_star(i) = eval(sprintf("reg.const_%s",controller_suffixes(i)));
   u_post_step (i) = eval(sprintf("reg.const_%s_post_step",controller_suffixes(i)));
end

%% Save everything, so somebody else can do system ID
step_amplitudes = num2cell(u_post_step - u_star);
step_time = step_times{1}; % They must all be the same
save("outputs/estimation_experiment_result.mat", "u_star", "constant_response", "step_responses", "step_amplitudes", "step_time")

