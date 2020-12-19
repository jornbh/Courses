function [lqe_params] = make_lqe(plant, Q_lqe,R_lqe, G, tau_anti_aliasing_filter)
if ~exist("G", "var")
   G = eye(length(plant.A));  
end
%% Linear quadratic state estimator
    % An lqe state estimator is suposed to be able to take in noisy measurements from a plant and find the expected states
    % The model states are suposed to be updated with 
    %       x_hat_dot = A*x_hat * B*u  - L_lqe*(C*x_hat - y)
%% Function body
    % The lqe can use more measurements and contineous time for better estimation
    A = plant.A; 
    C = plant.C; 


    %% Costs 
        % The matrices of the cost-function are based on the axpected co-variance between the noise-signals, or the disurbance-signals
        % Q = E[disturbance*disturbance']
        % R = E[ noise*noise']
        % I did not find any usefull patterns in the noise. Feel free to tweak these values
        % The ratio between the matrices will tell if we should trust the model more, or the measurements
    %The lqe solves the Ricatti equation, so that we do not have to think about it 
    [K_lqe] = lqe( A, G, C, Q_lqe,R_lqe);
    
    lqe_params.K_lqe = K_lqe; 
    lqe_params.observed_plant = plant; 
    lqe_params.anti_aliasing_filter = ss(tf(1, [tau_anti_aliasing_filter,1]) * eye(size(C,1))); % A filter is used on each output, also to prevent aliasing
    lqe_params.tau_anti_aliasing_filter = tau_anti_aliasing_filter;
    lqe_params.G_lqe = G; 
end