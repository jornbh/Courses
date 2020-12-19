%% set "original" parameters
original_discrete_agnostic_mpc_params = discrete_agnostic_mpc_params;
original_agnostic_pseudo_discrete_mpc_params = agnostic_pseudo_discrete_mpc_params;
original_agnostic_pseudo_discrete_mpc_params = agnostic_pseudo_discrete_mpc_params; 
original_agnostic_lqr_params = agnostic_lqr_params;

% Assuming originals 
original_diff_integrated_lqr_params = diff_integrated_lqr_params;
original_lqr_params = lqr_params;
original_pseudo_discrete_mpc_params = pseudo_discrete_mpc_params ;
%% An attempt to make a controller
% %% Create a PID-controller 
% 
% [butter_cuttof_b, butter_cuttof_a] = butter( 6, 0.01, "s");
% single_butter_filter = tf(butter_cuttof_b, butter_cuttof_a);
% single_butter_ss = ss(single_butter_filter); 
% butter_filter = eye(2) * single_butter_filter;
% PID_opt = pidtuneOptions( "DesignFocus", "disturbance-rejection"); 
% PC = butter_filter*mpc_params.controlled_plant;
% % Extract the three controlled parameters
% P = PC* [   1 ,0 ,0;
%             0 ,0 ,reg.F_w_ratio;
%             0 ,1 ,0;
%             0 ,0 ,reg.v_grate_ratio]; % PID plant 
% % P = blkdiag(tf(1,[1,0]), tf(1,[1,0]))*P; 
% C = tf(1) * zeros([3,2]); % PID controller
% 
% 
% C(2,1) =0; 
% P_closed = feedback( P , C); 
% P_aI2st = [1,0]*P_closed*[0;1;0]; 
% F_aI_regulator = pidtune( P_aI2st, "PID",PID_opt); 
% C(2,1) = F_aI_regulator; 
% 
% C(1,2) =0; 
% P_closed = feedback( P ,  C); 
% P_aOI2O2 = [0,1]*P_closed * [1;0;0];
% F_aII_regulator = pidtune(P_aOI2O2, "PID", 1e-3, PID_opt); 
% C(1,2) = F_aII_regulator;
% 
% 
% P_closed = feedback( P , C); 
% air_PIDs = eye(2,3)*C; 
% 
% % Control difference between primary and secondary air in PID
% air_diff_vec = - (1/reg.op_F_aII^2) .* [ reg.op_F_aI, -reg.op_F_aII];
% AB_pid = pidtune(single_butter_filter*air_diff_vec*air_PIDs*P*[0;0;1], "PID", 1e-3, PID_opt); 
% waste_PIDs = AB_pid .* air_diff_vec;
% complete_PID_regulator = [eye(2); waste_PIDs ]*air_PIDs; 
% 
% closed_feedback_loop = (feedback(P, complete_PID_regulator)*complete_PID_regulator);
% u_feedback_loop = (feedback( complete_PID_regulator, P));
% % 
% % figure 
% % step(closed_feedback_loop )
% % figure
% % step(closed_feedback_loop )
% % title("Bode y")
% % figure 
% % bode(u_feedback_loop )
% % title("Bode u")
% 
% model_tuned_PID = complete_PID_regulator; 
% 
% %% PID for block diagram 
% AB_pid = pid(AB_pid);
% F_aI_pid = pid(C(2,1));
% F_aII_pid = pid(C(1,2));
% %% Old controller (Test characteristics)
% O2_pid = pid(reg.Kp_O2, reg.Ki_O2); 
% steam_pid = pid(reg.Kp_st, reg.Ki_st, reg.Kd_st); 
% AB_pid = pid(reg.Kp_AB, reg.Ki_AB); 
% 
% 
% steam_filter = tf(1,[reg.T_st_LPF^2 sqrt(2)*reg.T_st_LPF 1]);
% O2_filter =    tf(1,[reg.T_O2_LPF^2 sqrt(2)*reg.T_O2_LPF 1]);
% AB_filter =    tf(1,[reg.T_AB_LPF^2 sqrt(2)*reg.T_AB_LPF 1]);
% 
% read_filter = blkdiag( steam_filter , O2_filter); 
% air_controller = blkdiag( steam_pid, O2_pid); 
% AB_controller = (1/reg.op_F_aII^2) .* [ reg.op_F_aI, -reg.op_F_aII]* AB_pid*AB_filter;
% translator = [0,1;
%               1,0;
%               AB_controller];
% % open_loop_controlled = read_plant*translator*air_controller;
% old_C = translator*air_controller*read_filter; 
% % step(feedback(P*old_C, eye(2)))


%% Old sandbox
% %% Phase analysis of the LQR 
% P = d2c(mpc_params.MPC_plant)
% obs_P = lqe_params.observed_plant; 
% K_r = mpc_params.K_lqr; 
% K_e = lqe_params.K_lqe; 
% nx = length(P.A); 
% C_phys = P.C(1:2,:); 
% tot_A = [ 
%         P.A- P.B*K_r, -P.B*K_r; 
%         zeros(size(P.A)), blkdiag(-K_e*obs_P.C, -1e4*ones( length(P.A) - length(obs_P.A))); 
% ];
% 
% tot_B = blkdiag(P.B, [K_e;zeros([6,size(K_e,2)])]); 
% tot_C = blkdiag(C_phys,C_phys); 
% tot_D = zeros(size(tot_C*tot_B)); 
% 
% tot_ss = ss(tot_A, tot_B, tot_C, tot_D); 
% 
% state_C = [eye(size(C_phys,1)), zeros(size(C_phys,1))];
% error_C = [zeros(size(C_phys,1)), eye(size(C_phys,1))];
% input_B = [eye(4); zeros([3, 4])];
% noise_B = [zeros([4,3]); eye(3)];
% 
% % noise_B = 
% [res, time] = impulse(state_C* tot_ss*noise_B ); 
% % tot_B = [
% % 
% % [res,time] = step(P.C* feedback(oP, K_r));
% % 
% figure
% subplot_no =0; 
% for i = 1:size(res,2)
%     for j = 1:size(res,3)
%         subplot_no = subplot_no +1; 
%         subplot(size(res,2), size(res,3), subplot_no)
%         plot(time, res(:, i,j) )
%     end
% end
% 
% 
% %% Analyze the disturbances 
% noise_series = T_aII; 
% ffq = ((abs(fft(noise_series.signals.values- (1-1e-2)*mean(noise_series.signals.values)))));
% w = linspace( 0, 2*pi/noise_series.time(end),length(noise_series.time));
% freq_power = ffq/length(w)
% figure
% semilogy(flip(w((length(ffq)/2:end)))-w(end)/2, cummax(freq_power(length(ffq)/2:end,:)));
% title("Sampling frequency is 1/300")
% 
% figure
% plot(noise_series .signals.values- mean(noise_series .signals.values))
% title("disturbance - mean"); 