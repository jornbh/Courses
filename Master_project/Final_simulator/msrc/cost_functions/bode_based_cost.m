%TODO See if this function is even used, and what the differences are
function cost = bode_based_cost(diff_mpc_params, lqe_params, expanded_approxomated_system, min_w_noise, max_w_dist, bode_cost_weights)
    % Optimization cost based on bode plots 
    % 
    % :param diff_mpc_params: A struct
    if ~exist("min_w_noise", "var")
        min_w_noise = 1;
    end

    full_P = expanded_approxomated_system;
    % P = full_P * eye(size(full_P.B,2),4); % Only the first 4 inputs are controlled
    % Reconstruct the controled plant
    P = d2c(diff_mpc_params.MPC_plant);
    % plant_with_diff = append(diff_mpc_params.controlled_plant, [-1,0,1,0])*[eye(size(P,2));eye(size(P,2))];
    % P = plant_with_diff * diff_mpc_params.input_filter;
    obs_P = lqe_params.observed_plant;
    K_r = diff_mpc_params.K_lqr;
    max_Kr = max(K_r, [], [1, 2])
    K_e = lqe_params.K_lqe;
    nx = length(P.A);
    C_phys = P.C(1:size(P.C, 1) / 2, :);

    if size(C_phys, 1) < 3
        C_phys = [C_phys; [-1, 0, 1, 0] * pinv(P.B)]; % Hacky diff o2 flow state
    end

    tot_A = [
        P.A - P.B * K_r, -P.B * K_r;
        zeros(size(P.A)), blkdiag(-K_e * obs_P.C, -1e4 * ones(length(P.A) - length(obs_P.A)));
        ];

    N_virtual_states = size(P.A, 1) - size(full_P.A, 1); % Controller filters and integrators
    tot_B = blkdiag([full_P.B(:, 5:end); zeros([N_virtual_states, size(full_P.B(:, 5:end), 2)])], [K_e; zeros([N_virtual_states, size(K_e, 2)])]);
    tot_C = blkdiag(C_phys, C_phys);
    tot_D = zeros(size(tot_C * tot_B));

    tot_ss = ss(tot_A, tot_B, tot_C, tot_D);

    N_disturbances = size(full_P.B(:, 5:end), 2);
    state_C = [eye(size(C_phys, 1)), zeros(size(C_phys, 1))];
    error_C = [zeros(size(C_phys, 1)), eye(size(C_phys, 1))];
    disturbance_B = [eye(N_disturbances); zeros([3, N_disturbances])];
    noise_B = [zeros([N_disturbances, 3]); eye(3)];

    %% Construct a cost
    noise_amplitudes = lqe_params.y_star * 1e-3; % Just a guess
    dist_variances = [1.572, 0.2387, 1e8 * [1.3751, 3.0121, 6.116]]; %Measured disturbances variances
    dist_variances = dist_variances((end - N_disturbances + 1):end);
    %% Calculate bode responses, given the power of the inputs

    % noise_B =
    freqs = 10.^linspace(-9, 4, 50);
    [dist_amp, dist_angle] = bode(state_C * tot_ss * disturbance_B * diag(dist_variances), freqs);
    [noise_amp, noise_angle, noise_freqs] = bode(state_C * tot_ss * noise_B * diag(noise_amplitudes), freqs);

    disturbance_band = dist_amp(:, :, freqs <= max_w_dist);
    noise_band = noise_amp(:, :, freqs >= min_w_noise);
    Tsum = @(x) sum(sum(disturbance_band, [2, 3]) .* bode_cost_weights');

    %% Disturbance cost

    disturbance_cost = Tsum(disturbance_band) / size(disturbance_band, 3) * 1e2;
    % This cost gives equal weight to everything, which might be bad
    noise_cost = Tsum(noise_band) / size(noise_band, 3); % Just some scaling to make the number of points in freqs matter less

    cost = noise_cost + disturbance_cost;
    %% Plot consequences
    %
    % figure(14)
    % figure(14)
    % subplot_no =0;
    % for i = 1:size(noise_amp,1)
    %     for j = 1:size(noise_am
    % subplot_no =0;
    % for i = 1:size(noise_amp,1)
    %     for j = 1:size(noise_amp,2)
    %         subplot_no = subplot_no +1;
    %         subplot(size(noise_amp,1), size(noise_amp,2), subplot_no)
    % %         plot(freqs , squeeze(noise_amp(i,j,:)) )
    %
    %         selected_noise_amp = squeeze(noise_amp(i,j,:));
    %         hold on
    %         plot(freqs , selected_noise_amp )
    %         plot(freqs(freqs>=min_w_noise) , squeeze(noise_band(i,j,:)) , "r--")
    %         set(gca, "XScale", "log")
    %         set(gca, "YScale", "log")
    %     end
    % end
    % sgtitle("Noise bode plot")

    % figure
    %
    % subplot_no =0;
    % for i = 1:size(dist_amp,1)
    %     for j = 1:size(dist_amp,2)
    %         subplot_no = subplot_no +1;
    %         subplot(size(dist_amp,1), size(dist_amp,2), subplot_no)
    %         selected_dist_amp = squeeze(dist_amp(i,j,:));
    %         hold on
    %         plot(freqs , selected_dist_amp )
    %         plot(freqs(freqs>=min_w_noise) , selected_dist_amp(freqs>=min_w_noise)  , "r--")
    %         hold off
    %         set(gca, "XScale", "log")
    %         set(gca, "YScale", "log")
    %     end
    % end
    % sgtitle("Disturbance bode plot")
