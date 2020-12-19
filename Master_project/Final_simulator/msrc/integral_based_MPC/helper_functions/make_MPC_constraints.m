% Set MPC upper and lower bounds 
% Limits for inputs
function [Ac,b0, x0_corr, slack_H] = make_MPC_constraints(disc_MPC_sys,  M, CONV,N_prediction_steps)
B = disc_MPC_sys.B; 
C = disc_MPC_sys.C; 

I_B = eye( size(B,2));

lower_u_limits = [ -11,-20,-18, -2];  
lower_y_limits = [-Inf, -3.5, -Inf]; 

I_N_steps = eye(N_prediction_steps); 
ones_N_steps = ones([N_prediction_steps,1]);

% Set input limits
A_u_lower = kron( I_N_steps, I_B);
b_u_lower = - kron(ones_N_steps, lower_u_limits'); 


% Set output limits
long_observer = kron(I_N_steps, C(1:size(C,1)/2,:)); % Physical states
A_y_lower = long_observer*CONV; 
b_y = long_observer*M; % 
b0_y_lower = - kron(ones_N_steps, lower_y_limits(1:size(C,1)/2)'); % Only include the relevant outputs


% Combine the constraints 
A_combined = [
            A_u_lower; 
            A_y_lower         
            ]; 

b0_combined = [b_u_lower; b0_y_lower];
% Upper bounds is not an issue, so they are ignored
A_slack = eye(size(b0_combined,1)); 
b0_slack = - zeros([size(b0_combined,1),1]);


total_A = [A_combined, A_slack;
      zeros(size(A_combined)), A_slack];
total_b0 = [b0_combined; b0_slack]; 
% Set limits on on the outputs

total_x_correction = [b_y; zeros(size(b_y))];

%% Assign the outputs 

Ac      = total_A;     
b0      = total_b0;     
x0_corr = total_x_correction;           
slack_H = 1e15 * eye(size(A_slack,1)); 
end

