%% Example input 
% [a,b] = create_MPC(); 

% lqe_params = b.agnostic_lqe_params; 
% controller_params = a.LQRDiffIntegralAgnostic_params; 
% 
% % lqe_params = b.agnostic_lqe_params;
% % controller_params = a.MPCDiscrete_params; 
% MPC_name_prefix = "LQRDiffIntegralAgnostic_params"; 
% robustness_based_costs(MPCDiscreteAgnostic_params, agnostic_lqe_params, "MPCDiscreteAgnostic")
function robustness_cost = robustness_based_costs(controller_params, lqe_params, MPC_name_prefix)
%% Itteration parameters
N_changes = 3;


%% Sanity check 

%% Evaluate the eigen-values in the feedback-loop when B changes
i =0; 

scaling_min = 0.7; 
scaling_max = 2.3;
max_eig_B = 0; 
% figure 
% hold on 
for B_scaling = linspace(scaling_min ,scaling_max , N_changes)
    i = i+1; 
    feedback_loop = make_big_ss(lqe_params, controller_params,B_scaling,1e-7, MPC_name_prefix);
    
    eigs = eig(feedback_loop.A); 
    max_eig_B= max(max_eig_B, max(real(eigs))/(1e-6+abs(B_scaling-1))); % Later problems are better
    
%     plot(i,max_eig_B, "o")

end

%% Root locus for time-delays
i =0; 
tau_min = 1e-9; 
tau_max = 3;
if contains(MPC_name_prefix, "Discrete")
    tau_max = 25;
end
max_eig_taus =0; 
for tau = linspace(tau_min,tau_max, N_changes)
    i = i+1; 
    feedback_loop = make_big_ss(lqe_params, controller_params,1,tau, MPC_name_prefix);
    
    eigs = eig(feedback_loop .A); 
    max_eig_taus = max(max_eig_taus, max(real(eigs))/tau); 
end

robustness_cost = max_eig_taus + max_eig_B; 

%% The glorious function
function feedback_loop = make_big_ss(lqe_params, controller_params, B_multiplier, time_delay, MPC_name_prefix)

%% The real work
% Matrix shorhand
Z = @(x,y) zeros([x,y]);
O = @(x,y) ones([x,y]); 
I = @(x,y) eye(x,y); 
%%Make shorhand
A = lqe_params.observed_plant.A;
B = lqe_params.observed_plant.B;
C = lqe_params.observed_plant.C;
Cc = blkdiag(eye(2,3)*lqe_params.observed_plant.C, ([-1,0,1,0])); % Controlled outputs 
D = lqe_params.observed_plant.D; 
Dc = [I(2,3)*lqe_params.observed_plant.D; Z(1,4)];

% Input filter
iF = controller_params.input_filter; 
Ke = lqe_params.K_lqe; 
Kr = controller_params.K_lqr; 
Nx = length(A);
Ny = size(C,1);
Nu = size(B,2); 
NycI = size(controller_params.MPC_plant.C,1); % Number of controlled states (Including integrators) 
Nyc = NycI/2; % Numpber of physical controlled states
N_filter_states = length(controller_params.input_filter);

%% Prepare variable parts
% Variable sub-systems 
    B_phys = B*B_multiplier; % May change
     [pade_num,pade_den] = pade(time_delay ,9); 
%      P = ss(eye(Nu)* tf(pade_num,pade_den)); %pade_ss
     % Air delay
%      P = ss((diag([1,0,1,0])*tf(pade_num,pade_den)) + diag([0,1,0,1])); % Only delay on air
     %waste delay
     P = ss((diag([0,1,0,0])*tf(pade_num,pade_den)) + diag([1,0,1,1])); % Only delay on air
     
     Np = size(P.A,1);
    


%% Make the big, undelayed system

 % With diff integral cost
 
 filter_dynamics = [Z(Nu,Nx)          , iF.A             , Z(Nu,Nyc)]; % All filter states
 if size(controller_params.controlled_plant.C,1) == 3
  % Physical, estimated, filter, integrated, pade
     big_A = [
         [A          ,  Z(Nx,Nx)             , B_phys*P.D*iF.C       , Z(Nx,Nyc)     , B_phys*P.C ];        % Physical system 
         [Ke*C       , (A -Ke*C)             , B*iF.C                , Z(Nx,Nyc)     , Z(Nx, Np) ];         % Estimated system
         [Z(Nu,Nx)   , (  filter_dynamics )                                          , Z(Nu, Np)];          % Internal filter states
         [eye(2,3)*C , Z(Nyc-1, Nx)            , eye(2,3)*D*P.D*iF.C   , Z(Nyc-1, Nyc)   , eye(2,3)*D*P.C];              % Output integrator minus air-diff
         [Z(1,Nx)    , Z(1,Nx)               , [-1,0,1,0]*P.D*iF.C   , Z(1 ,Nyc)     , [-1,0,1,0]*P.C ];    % Air diff integrator state
         [Z(Np,Nx)   , Z(Np,Nx)              , P.B*iF.C              , Z(Np,Nyc )    , P.A]; 
        ];
 else   
      big_A = [ %TODO This path is wrong
                 A          ,  Z(Nx,Nx)         , B_phys*iF.C      , Z(Nx,Nyc) ; % Physical system 
                 Ke*C       , (A -Ke*C)         , B*iF.C           , Z(Nx,Nyc) ; % Estimated system
                 Z(Nu,Nx)   , (  filter_dynamics ) ; % Internal filter states
                 Z(2,Nx)    , I(2,3)*C          , I(2,3)*Dc        , Z(2,Nyc)  ; % Output integrator minus air-diff
                ];
 end

big_C = Kr*[ Z(Nx+Nyc +Nu, Nx), eye(Nx+Nyc +Nu), Z(Nx+Nyc+Nu, Np)]; % Extract everything but the physical states and make it int inputs

big_D = Z(Nu, Nu); % No direct feed-through



big_B = [Z(Nx+Nx, Nu); iF.B; Z(Nyc,Nu); Z(Np, Nu)]; 

big_ss = ss(big_A, big_B, big_C, big_D); % Inputs to optimal inputs

feedback_loop = minreal(feedback( big_ss, eye(Nu))); % Simplified feedback loop

end
end