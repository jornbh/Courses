%% Parameters 
% input_indecies = [2]
% output_indecies = [1]
% constant_response 
% cell array if cell arrays "step_responses" (Output, Input)
% step_amplitudes = {1,1,1,1e-3}; 
% step_time 13e3
% u_star = {simout_F_aII.data(end), simout_F_w.data(end), simout_F_aI.data(end), simout_v_grate.data(end)}
function [approxomated_system, sigmas, tau_filter] = Step_method_main( constant_response, step_responses, step_amplitudes, step_time, model_order, u_star, dt)

input_indecies = 1:size(step_responses,2); %Lets you ignore some inputs or outputs 
output_indecies = 1:size(step_responses,1); % Deprecated and should be removed

if ~exist("u_star", "var")
    u_star = length(input_indecies); 
end


if ~exist("dt", "var")
    dt = 7; 
end


% Implcit parameters
n_inputs = length(input_indecies); 
n_outputs = length(output_indecies);
% actuation_type = 2; 
%% Make a matrix with raw inputs
T_end = constant_response{1}.time(end); % The start and end time should be the same for all 
T_start = constant_response{1}.time(1);
time =  T_start:dt:T_end;
all_outputs_uncorrected = zeros([length(time), n_outputs, n_inputs]);
for input_no = 1:n_inputs
    input_ind = input_indecies(input_no);
    tau_filter = dt; % A filter is added to prevent some of the aliasing
    oversample_time = T_start :0.1:T_end;
    for output_no = 1:n_outputs
        output_ind = output_indecies(output_no);
        raw_outputs = step_responses{output_ind,input_ind};
        oversampled_output = resample(raw_outputs, oversample_time); 
        resample_filter = tf([1],[tau_filter 1])^2;
        sprintf("Filtering: in: %i, out_ %i", input_ind, output_no)
        zero_init_output_data = oversampled_output.data' - oversampled_output.data(1); 
       [uncorrected_filtered, filtered_time]=lsim( resample_filter, zero_init_output_data , oversampled_output.time);
       filtered_ts = timeseries( uncorrected_filtered + oversampled_output.data(1), oversampled_output.time); 
       resampled_outputs =resample(filtered_ts , time);
       all_outputs_uncorrected(:,output_no,input_no) = resampled_outputs.data;  
    end
    
end
% all_outputs_uncorrected => Seems legit
%% Correct the problems with scaling (Step-response should have amplitude ~=1)
all_outputs = zeros(size(all_outputs_uncorrected)); 
for output_no = 1:n_outputs
    output_ind = output_indecies(output_no); 
    y_star(output_no) = constant_response{output_ind}.data(end,:); % The stationary value of some input 
end
for input_no = 1:n_inputs
    input_ind = input_indecies(input_no);
    input_amplitude = step_amplitudes{input_ind}; % The simulator crashed a lot,so small inputs were needed 
    for output_no = 1:n_outputs
        output_ind = output_indecies(output_no); 
        excitation = all_outputs_uncorrected(:, output_no, input_no) - y_star(output_ind); 
        all_outputs(:,output_no, input_no) = (excitation)/input_amplitude + y_star(output_ind); 
    end
end
%% Create a matrix of measurements


experiment_start = step_time; 
experiment_end = experiment_start + 2e4; 
within_experiment = (experiment_start <= time) & (time  < experiment_end);
% yFull is made by doing ( y(t) - y(t-1) ), which is dumb, but it works

% The diff experiment needs a measurement from before the step
within_difference_experiment = (find(within_experiment,1, "first")-1):find(within_experiment,1, "last");
yFull_step = all_outputs(within_difference_experiment, :,:) - y_star; 


yFull = diff(yFull_step); 
%% Correct for input amplitude
% The input amplitudes are all over the place, so adjusting them can be
% usefull
scaler_mat = squeeze( max(abs(yFull), [], 1)); 
original_scaler_mat = scaler_mat; 
% scaler_mat = squeeze(sum(abs(yFull(:,:,:)).^3,1).^(1/3));
output_scalers = ones([size(scaler_mat,1),1])
input_scalers = ones([size(scaler_mat,2),1]);
for scale_iteration = 1:4
    input_scalers(4) = input_scalers(4)/10; % Less priority to modelling the grate
    output_scalers(3) = output_scalers(3)/5; % And the HHV
    
    % Actual scaling
    scaler_mat = (original_scaler_mat.*output_scalers).*input_scalers';
    output_scalers = output_scalers./max(scaler_mat,[],2);
    scaler_mat = (original_scaler_mat.*output_scalers).*input_scalers';
    input_scalers = input_scalers./max(scaler_mat,[],1)';
    scaler_mat = (original_scaler_mat.*output_scalers).*input_scalers';
end

% % Prioritize the correctness of the controller inputs
% input_amplitudes(input_indecies) = input_amplitudes(input_indecies)./10; 
% output_amplitudes(output_indecies) = output_amplitudes(output_indecies) ./10
for input_no = 1:n_inputs
    for output_no = 1:n_outputs
        scaling = (output_scalers(output_no)*input_scalers(input_no)); 
        yFull(:,output_no,input_no) = yFull(:,output_no,input_no)*scaling; 
    end
end
yFull( isnan(yFull)) = 0; % 0/0 may happen, and that means there was a division by 0 => the signal was 0 
%% Perform the ERA 
"Start ERA"
[approxomated_system, sigmas, U,Sigma, V] = ERA(yFull, model_order, dt); 
"End ERA"

%% Revert the change in amplitude
yFull = diff(yFull_step);

approxomated_system.B = approxomated_system.B ./(input_scalers'); 
approxomated_system.D = approxomated_system.D ./(input_scalers'); 

approxomated_system.C = approxomated_system.C ./output_scalers;
approxomated_system.D = approxomated_system.D ./output_scalers; 

% approxomated_system.B = approxomated_system.B./dt; % Correct for time-step
%%
%% Verify the output 
% run compare_approx
%% Save the results
A = approxomated_system.A;
B = approxomated_system.B; 
C = approxomated_system.C;
D = approxomated_system.D;
dt =approxomated_system.Ts;
comments = "Remember that if an input-output pair don't affect each other much, then the estimate wil be bad";
% input_dir = get_result_dir(1,4);
% u_star = load(input_dir+"u.mat").u_excited.data(end,:);
% save("outputs/estimated_system_matrices.mat", "A","B","C","D","dt","u_star","y_star", "comments")


