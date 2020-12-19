run('common.m');

%% Initialize values from previous problems
% Part 2
[S_w, f] = pwelch(deg2rad(psi_w(2,:)), 4096, [], [], 10);
S_w = S_w ./ (2*pi);
omega = (2* pi) .* f;
w_0 = omega(find(S_w == max(S_w)));

sigma = sqrt(max(S_w));
S_w_analytic = @(lambda, omega) (2*lambda.*w_0.*omega*sigma).^2 ./ ...
    ((w_0.^2 - omega.^2).^2 +(2.*lambda.*w_0.*omega).^2);
lambda_0 = 0.1;
lambda = lsqcurvefit(S_w_analytic, lambda_0, omega, S_w);

K_w = 2 * lambda * w_0 * sigma;

%Part 4
A = [0 1 0 0 0; -w_0^2 -2*lambda*2 0 0 0; 0 0 0 1 0; 0 0 0 -1/T -K/T; 0 0 0 0 0];
B = [0; 0; 0; K/T; 0];
C = [0 1 1 0 0];
E = [0 0; K_w 0; 0 0; 0 0; 0 1];


%% Problem a

T_s = 0.1; % 10 Hz
[A_d, B_d] = c2d(A, B, T_s);
[A_d, E_d] = c2d(A, E, T_s);

%% Problem b
sim('P5b.slx');
measurement_noise_variance = var(compass.data);

%% Problem c
Q = [30 0; 0 10 ^(-6)];
P_0_pri = [1 0 0 0 0; 0 0.013 0 0 0; 0 0 pi^2 0 0; 0 0 0 1 0; 0 0 0 0 2.5*10^-3];
R = measurement_noise_variance / T_s;
x_0 = [zeros(10, 1); P_0_pri(:)];
data = struct('A', A_d, 'B', B_d, 'C', C, 'E', E_d, 'R', R, 'Q', Q, 'x_0', x_0);

% sys = idss(A, B, C, 0, E)