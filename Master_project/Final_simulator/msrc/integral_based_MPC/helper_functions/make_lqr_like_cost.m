function [H, F_y_ref, F_du, F_x0, K_lqry_c, K_lqry_d] = make_lqr_like_cost(disc_MPC_sys, N_prediction_steps,M,CONV, Qy, Qy_diff,R, Rd)
    % This function gives a positive definite cost matrix H and a function F which is used for the quadratic problem 
    % min_U  { Σ U^T * H * U + (F(x_0, y_ref, U_prev))^T *U  }
    %    Where U are all inputs over some prediction horizon. 
    % F is made by multiplying each input of the function with F_y_ref, F_du, F_x0 reespectively.     




%% Parameters: Cost of deviating from reference or changing the inputs

% Define shorthand for common matrices
    A = disc_MPC_sys.A;
    B = disc_MPC_sys.B;
    C = disc_MPC_sys.C;
    D = disc_MPC_sys.D; 

    %% Attempt of lqry-controll
    % Make an expanded C to handle the additional kinds of cost
        % Make a new discrete system, handling y, Δu and Δy
        low_pass_filter_indecies = ((length(A)-size(C,1)/2 - size(B,2)+1):(length(A)-size(C,1)/2)); 
        filter_C =  zeros([length(low_pass_filter_indecies), length(A)]);
        filter_C(:,low_pass_filter_indecies) = eye(length(low_pass_filter_indecies));
        diff_C = C( 1:(size(C,1)/2),:) - C( 1:(size(C,1)/2), :)*A;
        expanded_C = [C; filter_C; diff_C]; 
        
        % The cost of differentiation has to be spread between x_u and u
        Q_lqry = blkdiag( Qy, Rd, Qy_diff); % Joined cost, handling everything        
        R_lqry = R + Rd + B'*C'*blkdiag(Qy_diff, zeros(size(Qy_diff)))*C*B;
        N_lqry = - [
            zeros([size(C,1), size(B,2)]); 
            Rd;
           Qy_diff*C(1:size(Qy_diff,1),:)*B
            ];
        lqry_plant = ss(A,B,expanded_C,zeros([size(expanded_C,1), size(B,2)]),disc_MPC_sys.Ts);
        
        
        K_lqry_c = lqry(d2c(lqry_plant), Q_lqry, R_lqry, N_lqry); 
        failed_K_lqry_d = true; % Assigning K_lqry sometimes goes wrong if it is based on a controller that is too aggressive
        
        for attempt_no = 1:20
            if ~ failed_K_lqry_d
                break; 
            end
        try
            K_lqry_d = lqry((lqry_plant), Q_lqry, R_lqry, N_lqry);
            failed_K_lqry_d = false;
        catch ME
            ME
            disp("adding eye()*1e-20 to  [Q N;N' R] to make it positive definite")
            % The lqry failed because  [Q N;N' R] needs to be positive
            % definite. Adding a small extra cost to all states can fix
            % this
            correction = eye(length(Q_lqry) + length(R_lqry))*1e-24; % Add a small cost to everything, untill the matrix is P
            Q_lqry = Q_lqry  + correction(1:length(Q_lqry),1:length(Q_lqry));
            R_lqry = R_lqry + correction((length(Q_lqry)+1):end, (length(Q_lqry)+1):end);
        end
        end
        if failed_K_lqry_d == true 
            error("Was not able to find a proper matrix")
        end
        K_lqr = K_lqry_d; 
        Q_bar = dlyap(((A-B*K_lqr))', expanded_C'*Q_lqry*expanded_C+K_lqr'*R_lqry*K_lqr + expanded_C'*N_lqry*K_lqr); % Infinite horizon cost
        Q_bar = real(Q_bar); % Sometimes incorrect complex solutions sneak in
        Q_hat_direct = kron(eye(N_prediction_steps-1), expanded_C'* Q_lqry* expanded_C); % Cost of states within horixon
        Q_hat = blkdiag(Q_hat_direct, Q_bar); % Infinite horizon state cost (Preferably all constraints should be relaxen by then)
        Q_hat = Q_hat + 1e-9*eye(size(Q_hat)); % Enforce positive definiteness
        R_tot = R_lqry; 
        N_hat = kron(eye(N_prediction_steps),   expanded_C'*N_lqry); 
        
%%  Cost of change in input over N time-steps (Input  from the previous MPC-iteration is added in F)
    R_hat = kron(eye(N_prediction_steps), R_tot); % R_hat if the cost were on the states

% u(t) -u(t-1) can be represented as a matrix 
%     diff_base = diag(-ones([N_prediction_steps,1])) + diag(ones([N_prediction_steps-1,1]),-1);
%     diff_mat = kron( diff_base, eye(size(B,2))); % Take difference of the inputs ( u(0) is missing ) 


% Multiply with the old input
    get_first_u = diag([ones([size(B,2),1]); zeros([(N_prediction_steps-1)*size(B,2),1])]);

% Making a feed-forward cost is impossible for an integrating system
    % One solution is to use the feed forward term for the non-integrating system
    C_integrator = C(fix(size(C,1)/2)+1:end,:);
    B_integrator = C_integrator'* disc_MPC_sys.Ts; 
    ref_multiplications = (1:N_prediction_steps)';
    integrator_sumator = kron( ref_multiplications, B_integrator);

    
%% F will change depending on state, reference and previous inputs. Each part is represented by a separate matrix
%     CONV = (kron(eye(N_prediction_steps), expanded_C)*CONV);
    F_y_ref = CONV'*Q_hat*integrator_sumator; % Like an additional input to the system, building up the integrator states
    F_du =  zeros([(N_prediction_steps)*size(Rd,1), size(Rd,2)]); % Removed to focus on an lqr-like disign instead
    F_x0 = CONV'*Q_hat*M + (N_hat'*M); % Change in cost because of initial state


%% Finalize the costs for min_U {U'*H*U + F'(x0,y_ref,u_prev) }
    % For the product U'*H*U, the symetry should not be required, but it is
    % for chol()
    H = CONV'*Q_hat*CONV + R_hat + (CONV'*N_hat + CONV'*N_hat)./2;
    H = (H + H')./2; % H is iuposed to be symetric
%     F = @(x0, physical_y_ref, u_prev) (                  ...    
%                                 F_x0*x0         ...            
%                                 + F_y_ref*physical_y_ref ...                    
%                                 + F_du*u_prev  ...                
%                                 );





% K_lqr = K_lqry_c; % Use contineous feedback instead
end