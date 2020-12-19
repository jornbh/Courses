
function [approx_sys, sigmas, U, Sigma, V] = ERA( yFull, r, dt)
impulse_responses = yFull;
%impulse_responses = sym( "Y_", size(yFull(1:20,:,:)));

%impulse_responses = OKID_impulse_responses;
permuted = permute(impulse_responses(2:end, :,:),[2,3,1]); %[outputs, inputs, time]


[n_outputs, n_inputs, total_timesteps] = size(permuted);
n_steps = floor((size(permuted,3)-1)/2);
    

%H = sym("H", [n_steps*n_outputs, n_steps*n_inputs]); % H is measurements = C*A^i*B
H = zeros( [n_steps*n_outputs, n_steps*n_inputs]); % H is measurements = C*A^i*B
%H_base = sym("H_base", size(H) +[n_outputs, n_inputs]); % Makes H(0) and H(1)
H_base = zeros(size(H) +[n_outputs, n_inputs]); % Makes H(0) and H(1)
for i = 1:(n_steps+1)
    splitA = num2cell(permuted(:,:, i:(n_steps+i)) ,[1,2]);
    concatenated = (cat(2,splitA{:}));
    H_base((1 +(i-1)*n_outputs):((i)*n_outputs),:) =concatenated ;
end

H_prime = H; % 


[H_end_1, H_end_2] = size(H);
H(:,:) = H_base(1:H_end_1, 1:H_end_2);
H_prime(:,:) = H_base( (n_outputs+1):end, 1:H_end_2); % Sliding everything down one step is the same as moving one time-step forward

% This is the heavy part, and can be saved for later 
% Sigma can give a hint to how many states the model should use
global U Sigma V
[U, Sigma, V] = svd(H); % Singular value decomposition (V is not V^*  )


Sigma_tilde = Sigma(1:r,1:r);
U_tilde = U(:,1:r);
V_tilde = V(:,1:r);
% Truncate the values

Expanded_I_p = zeros([size(V_tilde,1), n_inputs]);
Expanded_I_q = zeros([n_outputs, size(U_tilde,1)]);

for i = 1:n_inputs
    Expanded_I_p(i,i) =1;
end

for i = 1:n_outputs
    Expanded_I_q(i,i) =1;
end

A_tilde = Sigma_tilde^(-1/2) *U_tilde'* H_prime*V_tilde* Sigma_tilde^(-1/2);
B_tilde = Sigma_tilde^(1/2)*V_tilde' * Expanded_I_p; %This may be incorrect
C_tilde = Expanded_I_q * U_tilde * Sigma_tilde ^(1/2);
D_tilde = squeeze(impulse_responses(1,:,:));

% The ERA uses unit impulse (Kronecker delta) responses, so the resulting system is
% incorrectly scaled for systems that were excited by an imitation of a
% dirac delta
B_tilde = B_tilde; 
D_tilde = D_tilde; 
sigmas = diag(Sigma); 
approx_sys = ss(A_tilde, B_tilde, C_tilde, D_tilde, dt);
end