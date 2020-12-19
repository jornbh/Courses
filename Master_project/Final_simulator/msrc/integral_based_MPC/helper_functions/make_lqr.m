
    
    cont_MPC_sys = d2c(disc_MPC_sys);
    Ad = disc_MPC_sys.A;
    Bd = disc_MPC_sys.B;
    Cd = disc_MPC_sys.C;
    Ds = disc_MPC_sys.D; 
    
    
    Ac = cont_MPC_sys.A;
    Bc = cont_MPC_sys.B;
    Cc = cont_MPC_sys.C;
    Dc = cont_MPC_sys.D; 

    low_pass_filter_indecies = ((length(Ad)-size(Cd,1)/2 - size(Bd,2)+1):(length(Ad)-size(Cd,1)/2)); 
    
    %% Contineous
    filter_C =  zeros([length(low_pass_filter_indecies), length(Ad)]);
    filter_C(:,low_pass_filter_indecies) = eye(length(low_pass_filter_indecies));
    diff_C = Cc( 1:(size(Cd,1)/2),:)*Ac;
    expanded_C = [Cc; filter_C; diff_C]; 
    
    Q_lqry = blkdiag( Qy, Rd, Qy_diff); % Joined cost, handling everything       
    R_lqry = R + Rd + Bc'*Cc'*blkdiag(Qy_diff, zeros(size(Qy_diff)))*Cc*Bc;
    N_lqry = - [
            zeros([size(Cd,1), size(Bd,2)]); 
            Rd;
           Qy_diff*Cc(1:size(Qy_diff,1),:)*Bc
            ];
    lqry_plant = ss(Ad,Bd,expanded_C,zeros([size(expanded_C,1), size(Bd,2)]),disc_MPC_sys.Ts);
    K_lqry_c = lqry(d2c(lqry_plant), Q_lqry, R_lqry, N_lqry); 

     %% Discrete 
    filter_C =  zeros([length(low_pass_filter_indecies), length(Ad)]);
    filter_C(:,low_pass_filter_indecies) = eye(length(low_pass_filter_indecies));
    diff_C = Cd( 1:(size(Cd,1)/2),:) - Cd( 1:(size(Cd,1)/2), :)*Ad;
    expanded_C = [Cd; filter_C; diff_C]; 
        
        % The cost of differentiation has to be spread between x_u and u
        Q_lqry = blkdiag( Qy, Rd, Qy_diff); % Joined cost, handling everything        
        R_lqry = R + Rd + Bd'*Cd'*blkdiag(Qy_diff, zeros(size(Qy_diff)))*Cd*Bd;
        N_lqry = - [
            zeros([size(Cd,1), size(Bd,2)]); 
            Rd;
           Qy_diff*Cd(1:size(Qy_diff,1),:)*Bd
            ];
        lqry_plant = ss(Ad,Bd,expanded_C,zeros([size(expanded_C,1), size(Bd,2)]),disc_MPC_sys.Ts);
        K_lqry_c = lqry(d2c(lqry_plant), Q_lqry, R_lqry, N_lqry); 
        K_lqry_d = lqry((lqry_plant), Q_lqry, R_lqry, N_lqry);
        K_lqr = K_lqry_d; 
        Q_bar = dlyap(((Ad-Bd*K_lqr))', expanded_C'*Q_lqry*expanded_C+K_lqr'*R_lqry*K_lqr + expanded_C'*N_lqry*K_lqr); % Infinite horizon cost
        Q_hat_direct = kron(eye(N_prediction_steps-1), expanded_C'* Q_lqry* expanded_C); % Cost of states within horixon
        Q_hat = blkdiag(Q_hat_direct, Q_bar); % Infinite horizon state cost (Preferably all constraints should be relaxen by then)
        Q_hat = Q_hat + 1e-9*eye(size(Q_hat)); % Enforce positive definiteness
        R_tot = R_lqry; 
        N_hat = kron(eye(N_prediction_steps),   expanded_C'*N_lqry); 
        