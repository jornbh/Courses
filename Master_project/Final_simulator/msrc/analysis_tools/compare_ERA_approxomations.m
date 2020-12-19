%#TODO Make sure this script functions properly (The created plots seem incorrect)
% Uncomment the line "run compare_approx" in "Step_method_main.m" for this script to work
close all
load('disturbance_estimation_experiment_result.mat')

%% Plot with inputs and disturbances
close all 
  for model_order = [5,7, 12,15]
    Step_method_main( constant_response, step_responses, step_amplitudes, step_time, model_order,u_star, 7); 
  end
 
figure(2)
subplot(3,7,1)
legend([ "5","7", "12","15","true"])

sgtitle("With disturbances") 
print(gcf, "outputs/Fig_dump/ERA_step_approx_with_disturbance_inputs", "-depsc")


%% Without disturbanes
close all
input_step_responses = {step_responses{:,1:4}};
input_step_responses  = reshape(input_step_responses, [3,4]);
input_step_amplitudes = {step_amplitudes{1:4}};

for model_order = [5,7, 12,15]
    Step_method_main( constant_response, input_step_responses, step_amplitudes, step_time, model_order,u_star, 7); 
end

figure(2)
subplot(3,4,1)
legend([ "5","7", "12","15", "true"])

sgtitle("Without disturbances") 
print(gcf, "outputs/Fig_dump/ERA_step_approx_without_disturbance_inputs", "-depsc")