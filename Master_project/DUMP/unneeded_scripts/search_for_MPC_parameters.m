run prepare_furnce
global approxomated_system
global u_star
global constant_response
global k
global reg
global mpc_function_type
global mpc_params
global step_fig
global progress_fig
global controlled_plant
global expanded_approxomated_system
FurnaceParameters
create_MPC % Initial guesses

reg.enforce_parameter_limits = true; 
% An old initial attempt to get good controllers

dummy_R =diag([
        1e-6;  % Secondary air
        1e-3;   % Waste-flow (Ram speed(?)) 
        1e-6;  % Primary air flow 1
        1e3;  % Grate speed (Very sensitive)
        ]*1e13); 
% Greatly punish the two kinds of air for going in the oposite direction
air_diff_cost = 1e-9;
air_ratio = 0.9; % Primary to secondary
dummy_R(1,1) = dummy_R(1,1) +air_diff_cost;
dummy_R(3,3) = dummy_R(3,3) +air_diff_cost*air_ratio^2;
dummy_R(1,3) =        -air_diff_cost*air_ratio;
dummy_R(3,1) =        -air_diff_cost*air_ratio;
    
dummy_Rd = diag([
        2e-6;  % Secondary air
        1e0;   % Waste-flow (Ram speed(?)) 
        3e-6;  % Primary air flow 1
        1e3;  % Grate speed (Very sensitive)
        ]*1e-12); 
dummy_Qy = 1e3*diag([1e4,1e3,1e7, 1e4]); % F_st, Y_O2, and the integrated versions 
dummy_Qy_diff  = 1e-12*diag([1e2,1e-12]); 
dummy_Q_lqe = diag([1.572, 0.238,1.3751e8, 3.0121e8, 6.116e8]); %Measured disturbances variances
% Q_input_disturbances = pinv(approxomated_system.B)'*diag([1e-2,1e-2,1e-4,1e-2])*pinv(approxomated_system.B);
% dummy_Q_lqe = dummy_Q_lqe + Q_input_disturbances;
dummy_R_lqe = diag([1e-9,1e-9,1e-3]); %


initial_attempt = log10((diag(blkdiag(...
     dummy_R + diag([air_diff_cost, air_diff_cost,0,0]) ,... 
     dummy_Rd,...
     dummy_Qy,...
     dummy_Qy_diff,...
     dummy_R_lqe,...
     dummy_Q_lqe, ...
     10^air_ratio,...
     air_diff_cost...
        )))); 



% % Use an empirically better cost instead
% options = optimset('PlotFcns',@optimplotfval);
options = optimset('PlotFcns',["optimplotfval"] );

%% Prepare a figure that can be ploted to 
% 
% initial_attempt = [ 
%     8.5230
%     1.8915
%     8.5365
%    10.2965
%   -30.0258
%   -14.9290
%   -14.5394
%   -13.3488
%     7.8622
%     6.2510
%    11.3692
%     8.4038
%     1.2016
%   -28.7979
%    -8.2965
%    -9.3100
%    -2.9902
%     0.1572
%    -0.8413
%     7.6669
%     7.7054
%     9.4129
%     0.9418
%    -7.8993
%     ];

% initial_attempt = [2.806751e-04, 4.063584e+00, 5.637587e-04, -3.651307e+00, 1.206047e+00, 6.730701e+00, 3.178554e+00, 9.835134e+00, -1.214764e-01, 1.319501e+00, 1.066214e+01, 6.723762e+00, 1.801357e+00, 1.479817e+00, 3.362446e+00, -7.039272e-01, 5.429686e+00, 3.203495e+00, 5.641542e+00, 5.540560e-01, 8.425748e+00, 3.292241e+00, 1.124742e+01, -1.652113e+00, -8.184091e-01, 3.162006e+00, 8.976341e+00, 9.795174e-01, 2.376363e-01]
% initial_attempt(1:8) = initial_attempt(1:8)+1; % A more carefull controller 
%% Search for the best parameters
best_params = fminsearch(@MPC_search, initial_attempt, options)
%% Test
MPC_search(best_params)
%%
function cost =  MPC_search(parameters)
    global approxomated_system
    global controlled_plant
    global u_star
    global constant_response
    global k
    global reg
    global mpc_function_type
    global mpc_params
    global lqe_params
    global gca % Current axis for ploting
    global step_fig
    global progress_fig
    global expanded_approxomated_system
    Nx = size(approxomated_system.A,1); 
    Nu = size(approxomated_system.B,2); 
    Ny = size(approxomated_system.C,1); 
    i =1; 
    Rd_base = parameters(i:(i+Nu-1));
    i = i + Nu; 
    R_base = parameters(i:(i+Nu-1));
    i = i + Nu; 
    Qy_base = parameters(i:(i+size(mpc_params.MPC_plant.C,1)-1));
    i = i + size(mpc_params.MPC_plant.C,1); 
    Q_y_diff_base = parameters(i:(i+size(mpc_params.MPC_plant.C,1)/2-1));
    i = i + size(mpc_params.MPC_plant.C,1)/2; 
    R_lqe_base = parameters(i:(i+Ny-1));
    i = i + Ny; 
    Q_lqe_base = parameters(i:(i + size(expanded_approxomated_system.B,2) - size(approxomated_system.B,2) -1));

   
    R = diag(10 .^R_base); 
    alpha = parameters(end-1); 
    diff_cost = parameters(end); % Choose a scaling between the two air-flows
    R(1,1) = R(1,1) +(10.^(diff_cost));
    R(3,3) = R(3,3) +(10.^(diff_cost))*alpha^2;
    R(1,3) =        -(10.^(diff_cost))*alpha;
    R(3,1) =        -(10.^(diff_cost))*alpha;
    Rd        = diag(10.^Rd_base);
    Qy       = diag(10.^Qy_base );
    Qy_diff = diag(10.^Q_y_diff_base);
    Q_lqe  = diag( 10.^Q_lqe_base); 
    R_lqe = diag( 10.^R_lqe_base); 
    try 

    %% Copy of create_MPC
    load('disturbance_estimation_experiment_result.mat')
    u_star_expanded = u_star;
    u_star = u_star(1:4); % Only the controllable inputs
    G_lqe = d2c(expanded_approxomated_system).B(:,5:end);
    G_lqe_measurements = G_lqe;% zeros(size(d2c(expanded_approxomated_system).B(:,5:end)));
    LQG_plant = d2c( approxomated_system); %The lqg can only work on contineous systems
    controlled_plant = eye(2,3)*LQG_plant; 
    
lqe_params = make_lqe(LQG_plant, Q_lqe, R_lqe, G_lqe); 
lqe_params.G_lqe = G_lqe; 
lqe_params.G_lqe_measurements = G_lqe_measurements;
lqe_params.integrator_direct_measurement_belief = 1; 
mpc_params = make_MPC_lqr_style(controlled_plant, Qy,Qy_diff, R, Rd, 20,20); 

% mpc_params = Main_MPC_lqr_style(approxomated_system, Qy,Qy_diff,R, Rd, Q_lqe, R_lqe,20,5, [1,2]); 
% mpc_params = Main_MPC_diff_style(approxomated_system, Qy,Qy_diff,R, Rd, Q_lqe, R_lqe,5,17, [1,2]); 
%%

diff_air_ss = ( tf(1,[1])); % No states will be added by this
extended_approxomated_system = append(controlled_plant,diff_air_ss)*[eye(4); [-1,0,1,0]];
diff_mpc_params = make_MPC_lqr_style(extended_approxomated_system, ...
        blkdiag(Qy(1:2,1:2),0,Qy(3:4,3:4), max(max([R(1,1),R(3,3)]))/1e4 ),...
        blkdiag(Qy_diff,0),...
        R,...
        Rd, ...
        20,5); 


lqe_params.u_star = u_star; 
for i = 1:3 
    lqe_params.y_star(i) = constant_response{i}.data(end); 
end 
lqe_params.v_star = u_star_expanded(5:end); % The constant disturbances to the plant

mpc_function_type = mpc_params.mpc_function_parameter_type;
mpc_params.mpc_ref = [0;0]; 

    
    unpack_mpc_params("mpc_params.mpc_function_params");
    save("last_mpc_params.mat", "mpc_params", "lqe_params", "diff_mpc_params");
    FurnaceParameters; 
    % Add disturbances 
    reg.input_type_Q_grate = InputType.Experimental;
    reg.input_type_T_aI = InputType.Experimental;
    reg.input_type_T_aII = InputType.Experimental;
    reg.controller_type = ControllerType.LQR; 
    reg.include_noise = true; 
   
%     sim("Furnace"); 
%   
%     steam_error = squeeze((abs(resample(simout_F_st, simout_F_st.time(1):1:simout_F_st.time(end)).data) - lqe_params.y_star(1))); 
%     O2_error =  squeeze((abs(resample(simout_F_O2, simout_F_O2.time(1):1:simout_F_O2.time(end)).data) - lqe_params.y_star(2))); 
%     air_diff_error = (squeeze((abs(resample(simout_F_aI - simout_F_aII, simout_F_O2.time(1):1:simout_F_O2.time(end)).data) - (lqe_params.u_star(3)- lqe_params.u_star(1)))));
%     overshoot_cost = (norm(steam_error, Inf) + 10*norm(O2_error, Inf)+ 0.01*norm(air_diff_error,Inf)); 
%     L2_norm_cost  = ( (norm(steam_error,2) + 50*norm(O2_error,2)+ 0.5*norm(air_diff_error,2)) /length(steam_error)) ; 
%     const_error_cost = (0.01*abs(sum(steam_error)) + abs(sum(O2_error)))/length(steam_error); % Punish if the system does not converge to reference properly
%     cost = overshoot_cost + L2_norm_cost+ const_error_cost; 
%     
%     
    %% Alternative cost 
    cost = bode_based_cost(diff_mpc_params, lqe_params, expanded_approxomated_system); 

    catch ME
        ME
        cost = 1e8; % A big punishment for unstability
    end
       
    
    
    sprintf( "Cost: %i <- %s",cost,  sprintf("%.12f, ", parameters))
end