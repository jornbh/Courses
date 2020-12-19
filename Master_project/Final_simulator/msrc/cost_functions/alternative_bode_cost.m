%%
% 
% diff_mpc_params
% lqe_params
% expanded_approxomated_system
% min_w_noise
% max_w_dist
% bode_cost_weights
function [disturbance_cost, noise_cost] = alternative_bode_cost(diff_mpc_params, lqe_params, expanded_approxomated_system, min_w_noise, max_w_dist, bode_cost_weights)

Af=expanded_approxomated_system.A;
Bf=expanded_approxomated_system.B;
Cf=expanded_approxomated_system.C;
Df=expanded_approxomated_system.D;

% Add a small, unexpected imperfection 
Bf = Bf +1e-9; 

Ao=lqe_params.observed_plant.A;
Bo=lqe_params.observed_plant.B;
Co=lqe_params.observed_plant.C;
Do=lqe_params.observed_plant.D;


Afi = diff_mpc_params.input_filter.A;
Bfi = diff_mpc_params.input_filter.B;
Cfi = diff_mpc_params.input_filter.C;
Dfi = diff_mpc_params.input_filter.D;

Ke = lqe_params.K_lqe;
Kr = diff_mpc_params.K_lqr; 

Nxf = length(Af); 
Nxo = length(Ao); 
Nyo = size(Co,1);
Nv = size(expanded_approxomated_system.B,2) -4; 
Nu = 4; 
NyI = size(diff_mpc_params.controlled_plant.C,1); 
Z = @(x,y) zeros([x,y]);
I = @(x,y) eye(x,y); 
big_A = [
 Af , Z(Nxf,Nxo), -Bf*I(7,4)*Cfi, Z(Nxf, NyI);
 Ke*Cf, (Ao - Ke*Co), (-Bo*Cfi), Z(Nxo, NyI);
 Z(Nu,Nxf), ((-Bfi*Kr) + [Z(Nu,Nxo), Afi, Z(Nu,NyI)]); % A bit more tricky because of feedback
 I(2,3)*Cf, Z(2,Nxo), I(2,3)*Df(:,1:Nu), Z(2,NyI); % Normal integrator states
 
];
if NyI == 3 %Add extra integrator if air diff cost
    tail = [
        Z(1,Nxf), Z(1,Nxo), [-1,0,1,0]*Cfi, Z(1,NyI);
    ];
    big_A = [big_A; tail];
end
big_B = [
            blkdiag(expanded_approxomated_system.B(:,5:end), Ke); % physical and estimate
            Z(Nu,Nv+Nyo); % Filter 
            Z(2,Nv), I(2,Nyo)
            ];
if NyI == 3
    tail = Z(1,Nv+Nyo);
    big_B = [big_B;tail];
end

big_C = Z(NyI, size(big_A,1));
big_C(1:2,1:Nxf) = I(2,3)*Cf;
if NyI ==3 % The extra integra air diff state as well 
    big_C(3,(Nxf+Nxo+1):(Nxf+Nxo+Nu)) = [-1,0,1,0];
end
big_D = Z(size(big_C,1),size(big_B,2));
big_ss = ss(big_A, big_B, big_C, big_D);

%% Calculate a cost 

freqs = 10.^linspace(-9,4, 50); 
[amps, angles, freqs] = bode(big_ss, freqs);

dist_amp = amps(:,1:Nv,:); 
noise_amp = amps(:,(Nv+1):end,:);

disturbance_band = dist_amp(:,:,freqs <= max_w_dist);
noise_band = noise_amp(:,:,freqs >= min_w_noise); 

disturbance_weights = -log10(freqs(freqs <= max_w_dist));% Punish low frequencies more
disturbance_tot= zeros([size(disturbance_band,1),size(disturbance_band,2)]);
for i = 1:size(disturbance_band,3)
    disturbance_tot = disturbance_tot + disturbance_weights(i)*disturbance_band(:,:,i);
end
% disturbance_sum = sum(disturbance_band*,[3]);
noise_sum = sum(noise_band, [3]); 
noise_amplitudes = lqe_params.y_star*1e-3; % Just a guess
dist_variances = [1.572, 0.2387,1e8*[1.3751, 3.0121, 6.116]]'; %Measured disturbances variances
dist_variances = dist_variances((end-Nv+1):end);

disturbance_cost = bode_cost_weights(1:NyI)*disturbance_tot*dist_variances;
noise_cost = bode_cost_weights(1:NyI)*noise_sum*noise_amplitudes;


% bode_cost = disturbance_cost + noise_cost; 


% %% Plot the bode-diagram 
% figure
% [res, freqs] = bodemag(big_ss, freqs);
% input_names = ["Q_{grate,1}", "Q_{grate,2}","Q_{grate,3}", "\epsilon_{F_{st}}", "\epsilon_{F_{O_2}}", "\epsilon_{HHV}"];
% output_names = ["F_{st}", "F_{O_2}", "HHV"]; 
% for i = 1:(Nyo+Nv)
%     subplot(3,Nyo+Nv, i)
%     title(input_names(i))
% end


% for i = 1:3
%     subplot(3,Nyo+Nv, i)
%     xlabel(output_names(i)); 
% end 

% for i = 1:(Nyo+Nv)
%     for j = 1:3
%     subplot(3,Nyo+Nv, i + (Nyo+Nv)*(j-1))
%     plot(freqs, res)
%     set(gca, "YScale", "log")
%     set(gca, "XScale", "log")
%     end
% end
end
% size(big_A)
% size(tail)