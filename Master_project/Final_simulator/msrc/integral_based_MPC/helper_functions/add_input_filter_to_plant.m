function [filtered_plant, input_filter] = add_input_filter_to_plant(plant, filter_time_constant)
% This enforces properness (Kills feed-through), which is (often) needed an MPC
    % The old states are in the topmost block of the matrix
    % All inputs are low-pass filtered by the same kind of transfer-function. 
    
    %
    %         1
    % ------------------
    %   τ*s    +    1
    %

    % input_filter_array is the same filter, so that it can be used in simulink, etc. 


N_inputs = size(plant,2);


% input_filter        = ss(series(eye(N_inputs), tf(1, [filter_time_constant ,1 ]))); 
input_filter = ss( -eye(N_inputs)./filter_time_constant, eye(N_inputs)./filter_time_constant, eye(N_inputs), zeros(N_inputs) );
% The filter can simpply be added by multiplying the filter with the state-space
    % This is the same as connecting them in series 
    %
    %     inputs ------> (plant* input_filter) -------------> outputs 
    %
    % Is the same as: 
    %
    %     inputs ------> input_filter ------>   plant ------> outputs 
    %


filtered_plant = plant*input_filter; 


% If F is the subspace representing the filter, then the new system is on the form: 

%                          [            ]     [      ]
%         x_extended_dot = [   A    B   ]     [   0  ]
%                          [   0    F   ]  +  [   1  ]
%                          [            ]     [      ]
%         
%         y_filtered     = [C  D]* x_extended 
%




% This was shorter than expected :^) 