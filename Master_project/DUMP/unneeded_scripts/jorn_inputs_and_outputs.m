close all 
clear
load('all_simouts.mat');
null_simout; % The simout that gave us the stationary values
step_simouts; % The simout we use
% The sampling-time is the same within the same simulation for some reason
simout_no = 1; 


MPC_dt = 7;% 1 second seams reasonable (Maybe) 
T_end = null_simout.Y_O2.time(end)
experiment_end = t_STEP + 5e4; % This seams like a reasonable time to end
% Plant outputs 
% Y_O2 
% F_steam
% HHV_out
yFull_step = zeros( [length(t_STEP:MPC_dt:experiment_end),3,4]); 
for input_ind = 1:4
    step_response = [
        step_simouts{input_ind}.F_steam;
        step_simouts{input_ind}.Y_O2;
        step_simouts{input_ind}.HHV_out;
    ];

    % null_response = 
    null_response = [
        null_simout.F_steam;
        null_simout.Y_O2;
        null_simout.HHV_out;
    ];
    for output_ind = 1:3
        
        relevant_samples      = resample(getsampleusingtime(  step_response(output_ind), t_STEP, Inf), t_STEP:MPC_dt:experiment_end);
        relevant_null_samples = resample(getsampleusingtime(  null_response(output_ind), t_STEP, Inf), t_STEP:MPC_dt:experiment_end);
        relevant_step = relevant_samples- relevant_null_samples; 
        yFull_step(:, output_ind, input_ind) = relevant_step.data; 
%         figure
%         plot(relevant_step)
%         title(sprintf("In: %i, Out: %i", input_ind, output_ind))    
    end
end
%% Parameter estimation
yFull = diff(yFull_step);
yFull_normalizer = squeeze(sum(abs(yFull).^2,1).^(1/2)); 
output_scalers = mean(yFull_normalizer,2); 
input_scalers = mean(yFull_normalizer./output_scalers,1); 
yFull_scaled = yFull;
for output_ind =1:size(yFull,2)
    for input_ind =1:size(yFull,3)
        yFull_scaled(:, output_ind, input_ind) = yFull(:, output_ind, input_ind)/ (input_scalers(input_ind) * output_scalers(output_ind));
    end
end
[approxomated_system_scaled, sigmas] = ERA(yFull_scaled, 3, MPC_dt);
approxomated_system = diag( output_scalers) * approxomated_system_scaled * diag(input_scalers);

output_names = [ "F_{steam}", "Y_{O2}", "HHV_{out}"];
input_names = ["F_{a,II}", "F_{w}", "F_{a,I}", "grate speed"];
yFull_time = 0:MPC_dt:(experiment_end - t_STEP - MPC_dt);

approxomation_impulse = impulse(approxomated_system , yFull_time); 
approxomation_step = step(approxomated_system , yFull_time); 
% %% Plot step 
% figure
% for input_ind = 1:4
%     for output_ind = 1:3
%         subplot(3,4,4*(output_ind-1) +input_ind)
%         hold on 
%         plot(yFull_time, yFull_step(1:end-1, output_ind, input_ind), "bx-");
%         plot(yFull_time, approxomation_step(:,output_ind, input_ind)); 
% %         hold off
%         legend("real", "approx")
%         title(sprintf("Input: %s, Output: %s", output_names(output_ind),input_names(input_ind)  ) )
%     end
% end

% %% Plot impulse
% figure 
% for input_ind = 1:4
%     for output_ind = 1:3
%         subplot(3,4,4*(output_ind-1) +input_ind)
%         hold on 
%         plot(yFull_time, yFull(:, output_ind, input_ind), "bx-");
%         plot(yFull_time, (MPC_dt)*approxomation_impulse(:,output_ind, input_ind)); 
% %         hold off
%         legend("real", "approx")
%         title(sprintf("Input: %s, Output: %s", output_names(output_ind),input_names(input_ind)  ) )
%     end
% end


%% Prepare models 
model_orders = [3,4,5,7,10];

for model_order = model_orders % [2,3,5,7,10,15,20]
    % [approxomated_system, sigmas] = ERA(yFull, model_order, MPC_dt);

    % Use scaled versions instead to get a better approxomation
    [approxomated_system_scaled, sigmas] = ERA(yFull_scaled, model_order, MPC_dt);
    approxomated_systems{model_order} = diag( output_scalers) * approxomated_system_scaled * diag(input_scalers);
end


%% Plot differences

figure
% Plot true series
for output_ind = 1:3
    for input_ind = 1:4
        subplot(3,4,4*(output_ind-1) +input_ind)
        plot(yFull_time, yFull_step(1:end-1, output_ind, input_ind), "x")
    end
end
%Plot approxomations
for model_order = model_orders % [2,3,5,7,10,15,20]
    % [approxomated_system, sigmas] = ERA(yFull, model_order, MPC_dt);

%     % Use scaled versions instead to get a better approxomation
%     [approxomated_system_scaled, sigmas] = ERA(yFull_scaled, model_order, MPC_dt);
%     approxomated_system = diag( output_scalers) * approxomated_system_scaled * diag(input_scalers);

    approxomation_step = step(approxomated_systems{model_order} , yFull_time); 

    for output_ind = 1:3
        for input_ind = 1:4
            subplot(3,4,4*(output_ind-1) +input_ind)
            hold on 
%             plot(yFull_time, (1/MPC_dt)*yFull(:, output_ind, input_ind), "x")
%             plot(yFull_time, approxomation_impulse(:,output_ind, input_ind));
%             plot(yFull_time, yFull_step(1:end-1, output_ind, input_ind)- (1/MPC_dt)*approxomation_step(:,output_ind, input_ind)); 
%             plot(yFull_time, yFull_step(1:end-1, output_ind, input_ind), "x")
            plot(yFull_time, approxomation_step(:, output_ind, input_ind))
            
            
%             title(sprintf("%s \\Rightarrow  %s,",input_names(input_ind) , output_names(output_ind) ) )
        end
    end

end
subplot(3,4,1)
legend(["True", string(model_orders)])

for input_ind = 1:4
    subplot(3,4, input_ind);
    title(input_names(input_ind))
end
for output_ind = 1:3
   subplot(3,4, (output_ind-1)*4 +1 )
   ylabel(output_names(output_ind))
end


%% Plot sigmas


semilogy(sigmas(1:30)./ sum(sigmas))
xlabel("Model order"); 






