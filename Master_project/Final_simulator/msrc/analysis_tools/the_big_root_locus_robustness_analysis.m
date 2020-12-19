%% Example input 
% [a,b] = create_MPC(); 
% lqe_params = b.agnostic_lqe_params; 
% controller_params = a.LQRDiffIntegralAgnostic_params; 

% lqe_params = b.agnostic_lqe_params;
% controller_params = a.MPCDiscrete_params; 
% MPC_name_prefix = "LQRDiffIntegralAgnostic_params"; 

function the_big_root_locus_robustness_analysis(lqe_params, controller_params, MPC_name_prefix)
%% Itteration parameters
N_changes = 200;
colours = jet(N_changes);

%% Sanity check 
% sanity_ss = make_big_ss(lqe_params, controller_params,1,1e-7); 
% step(sanity_ss)
% "DONE"
%% Evaluate the eigen-values in the feedback-loop when B changes
figure(6)
close(6)
i =0; 
figure(6)
subplot(2,1,1)
hold on 
subplot(2,1,2)
hold on
scaling_min = 0.5; 
scaling_max = 3,
color_ind =0; 
for B_scaling = linspace(scaling_min ,scaling_max , N_changes)
    color_ind = color_ind+1; 
    feedback_loop = make_big_ss(lqe_params, controller_params,B_scaling,0);
    
    eigs = eig(feedback_loop .A); 
    subplot(2,1,1)
    plot(real(eigs), imag(eigs), "o", "Color", colours(color_ind,:))
    xlim([-3e0,0.1+max(real(eigs))])
    subplot(2,1,2)
    if max(real(eigs)) > 0
        plot(B_scaling, max(real(eigs)), "or")
    else
        plot(B_scaling, max(real(eigs)), "ob")
    end
    xlabel("B-scaling")
    ylabel("Largest eigenvalue") 

%     set(gca, "XScale", "log")
end
hold off
cb = colorbar; 
ylabel(cb,'B-multiplicator') 
caxis([scaling_min ,scaling_max ])

print("JornDir/Fig_dump/B_scaling_root_locus"+MPC_name_prefix, "-depsc")

%% Root locus for time-delays
figure(7)
close(7)
color_ind =0; 
figure(7)
tau_min = 0; 
tau_max = 30;
subplot(2,1,1)
hold on 
subplot(2,1,2)
hold on
for tau = linspace(tau_min,tau_max, N_changes)
    color_ind = color_ind+1;
    feedback_loop = make_big_ss(lqe_params, controller_params,1,tau);
    
    eigs = eig(feedback_loop .A); 
    subplot(2,1,1)
    plot(real(eigs), imag(eigs), "o", "Color", colours(color_ind,:))  
    xlim([-3e0,0.1+max(real(eigs))])
    subplot(2,1,2)
    if max(real(eigs)) > 0
        plot(tau, max(real(eigs)), "or")
    else
        plot(tau, max(real(eigs)), "ob")
    end
    xlabel("Time-delay")
    ylabel("Largest real eigenvalue") 
    
%     set(gca, "XScale", "log")
end
hold off
cb = colorbar; 
ylabel(cb,'time-delay') 
caxis([tau_min,tau_max])
print( "JornDir/Fig_dump/tau_root_locus"+MPC_name_prefix +"air_diff", "-depsc")

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
     P = ss(eye(Nu)* tf(pade_num,pade_den)); %pade_ss
     % Air delay
%     P = ss((diag([1,1,1,0])*tf(pade_num,pade_den)) + diag([0,0,0,1])); % Only delay on air
%           P = ss((diag([1,0,1,0])*tf(pade_num,pade_den)) + diag([0,1,0,1])); % Only delay on air
     %waste delay
%      P = ss((diag([0,1,0,0])*tf(pade_num,pade_den)) + diag([1,0,1,1])); % Only delay waste ram
     P = ss((diag([0,1,0,1])*tf(pade_num,pade_den)) + diag([1,0,1,0])); % Only delay waste ram
%      
%      P = ss((diag([0,0,0,1])*tf(pade_num,pade_den)) + diag([1,1,1,0])); %
%      Only delay on grate speed
    if time_delay == 0
        P = ss(eye(Nu));
    end
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
i =0; 

scaling_min = 0.5; 
scaling_max = 3;
max_eig_B = 0; 

big_C = Kr*[ Z(Nx+Nyc +Nu, Nx), eye(Nx+Nyc +Nu), Z(Nx+Nyc+Nu, Np)]; % Extract everything but the physical states and make it int inputs

big_D = Z(Nu, Nu); % No direct feed-through



big_B = [Z(Nx+Nx, Nu); iF.B; Z(Nyc,Nu); Z(Np, Nu)]; 

big_ss = ss(big_A, big_B, big_C, big_D); % Inputs to optimal inputs

feedback_loop = minreal(feedback( big_ss, eye(Nu))); % Simplified feedback loop

end
end