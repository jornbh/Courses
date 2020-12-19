function [H, F_y_ref, F_du, F_x0, K_lqry_c, K_lqry_d] = make_diff_style_cost(disc_MPC_sys, N_prediction_steps,M,CONV, Qy, Qy_diff,R, Rd)
%% BUGS!
    % This function sometimes gives bad results, since numerical errors break the positive semi-definiteness of matrix-multiplications, like C'*Qy*C
    %% Shorthand 
    A = disc_MPC_sys.A;
    B = disc_MPC_sys.B;
    C = disc_MPC_sys.C;
    D = disc_MPC_sys.D; 


    % The lqr-like cost /should/ act similairly to the diff-based one, so that will be used to find the infinite horizon cost
    [~, ~, ~, ~, K_lqry_c, K_lqry_d] = make_lqr_like_cost(disc_MPC_sys, N_prediction_steps,M,CONV, Qy, Qy_diff,R, Rd);


    diff_base =  diag(ones([N_prediction_steps-0, 1])    )  ...
    -diag(ones([N_prediction_steps-1, 1]) , 1) ...
    -diag(ones([N_prediction_steps-1, 1]) ,-1); 

    %% State cost ( There are damgers with matrix-products becoming non-positive semidefinite) 
    Q_direct_cost = kron(eye(N_prediction_steps-1), C'*Qy*C); 
    Q_diff_cost = kron(diff_base, C'*blkdiag(Qy_diff, zeros(size(Qy_diff)))*C); % The diff constraints do not apply to the integrated variables
    

    
        low_pass_filter_indecies = ((length(A)-size(C,1)/2 - size(B,2)+1):(length(A)-size(C,1)/2)); 
        filter_C =  zeros([length(low_pass_filter_indecies), length(A)]);
        filter_C(:,low_pass_filter_indecies) = eye(length(low_pass_filter_indecies));
        diff_C = C( 1:(size(C,1)/2), :)*A - C( 1:(size(C,1)/2),:);
        expanded_C = [C; filter_C; diff_C]; 
        
        % Method copied from the differential-like method
        Q_lqry = blkdiag( Qy, Rd, Qy_diff); % Joined cost, handling everything        
        R_lqry = R + Rd + B'*C'*blkdiag(Qy_diff, zeros(size(Qy_diff)))*C*B;
        N_lqry = - [
            zeros([size(C,1), size(B,2)]); 
            Rd;
           Qy_diff*C(1:size(Qy_diff,1),:)*B
            ];
        lqry_plant = ss(A,B,expanded_C,zeros([size(expanded_C,1), size(B,2)]),disc_MPC_sys.Ts);
        K_lqry_c = lqry(d2c(lqry_plant), Q_lqry, R_lqry, N_lqry); 
        K_lqry_d = lqry((lqry_plant), Q_lqry, R_lqry, N_lqry);
        K_lqr = K_lqry_d; 
        Q_bar = dlyap(((A-B*K_lqr))', expanded_C'*Q_lqry*expanded_C+K_lqr'*R_lqry*K_lqr + expanded_C'*N_lqry*K_lqr); % Infinite horizon cost
        
        
    Q_bar = dlyap(((A-B*K_lqry_d))', expanded_C'*Q_lqry*expanded_C+K_lqry_d'*R_lqry*K_lqry_d + expanded_C'*N_lqry*K_lqry_d); % Infinite horizon cost
    Q_hat = Q_diff_cost + blkdiag( Q_direct_cost, Q_bar); % State costs


    %% Input cost
    R_direct_cost = kron( eye(N_prediction_steps), R ); 
    R_diff_cost   = kron( diff_base,  Rd); 

    R_hat = R_direct_cost + R_diff_cost; 



    % get_first_u = diag([ones([size(B,2),1]); zeros([(N_prediction_steps-1)*size(B,2),1])]);


    % Make an MPC that integrates the reference
    C_integrator = C(fix(size(C,1)/2)+1:end,:);
    B_integrator = C_integrator'* disc_MPC_sys.Ts; 
    ref_multiplications = (1:N_prediction_steps)';
    integrator_sumator = kron( ref_multiplications, B_integrator);


    F_y_ref = CONV'*Q_hat*integrator_sumator; % Like an additional input to the system, building up the integrator states
    F_du = -[Rd; zeros([(N_prediction_steps-1)*size(Rd,1), size(Rd,2)])]; % Correct with the previous input
    F_x0 = CONV'*Q_hat*M; % Change in cost because of initial state

    H = CONV'*Q_hat*CONV + R_hat ;
    H = (H + H')./2; % Enforce symetry to get positive definitenes
    
    if min(real(eig(H)))
        error("Matrix H was not positive seim-definite. Check if Qy,R,Rd and Qy_diff are positive definite. If they are, see the bug about C'*Qy*C not being positve-semi-definite (You may be on your own)")
    end