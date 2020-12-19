clear;
close all;
load('wave.mat');
%% Part 1
omega_1 = 0.005;
omega_2 = 0.05;

% Problem b
sim('P1b_1.slx');
A_1 = (max(compass1b_1.data) - min(compass1b_1.data(1000:end,1))) / 2;

T = sqrt(((A_2 * omega_2)^2 - (A_1 * omega_1)^2)/(A_1^2*omega_1^4 - A_2^2 * omega_2^4));
K = A_1 * omega_1 * sqrt(1 + omega_1 ^2 * ((A_2 * omega_2)^2 - (A_1 * omega_1)^2)/(A_1^2*omega_1^4 - A_2^2 * omega_2^4));

s = tf('s');
h_sys = (K)/(s*(1+T*s));

%% Part 2
% Problem a
[S_w, f] = pwelch(deg2rad(psi_w(2,:)), 4096, [], [], 10);
S_w = S_w ./ (2*pi);
omega = (2* pi) .* f;
w_0 = omega(find(S_w == max(S_w)));

% Problem d
sigma = sqrt(max(S_w));
S_w_analytic = @(lambda, omega) (2*lambda.*w_0.*omega*sigma).^2 ./ ...
    ((w_0.^2 - omega.^2).^2 +(2.*lambda.*w_0.*omega).^2);
lambda_0 = 0.1;
lambda = lsqcurvefit(S_w_analytic, lambda_0, omega, S_w);
K_w = 2 * lambda * w_0 * sigma;

%% Part 3

rudder_constraint = 45;
% Problem a
w_c = 0.1;
Pm = 50;
T_d = T;

T_f = tan(deg2rad(90 - Pm)) / w_c;
K_pd = w_c*sqrt(T_f^2*w_c^2 + 1)/ K;
h_pd = K_pd*(1 + T_d*s)/(1+ T_f*s);
sys_pd = h_sys* h_pd;
%% Part 4
% Problem a
A = [0 1 0 0 0; -w_0^2 -2*lambda*w_0 0 0 0; 0 0 0 1 0; 0 0 0 -1/T -K/T; 0 0 0 0 0];
B = [0; 0; 0; K/T; 0];
C = [0 1 1 0 0];
E = [0 0; K_w 0; 0 0; 0 0; 0 1];

%% Part 5

% Problem a
T_s = 0.1; % 10 Hz
[A_d, B_d] = c2d(A, B, T_s);
[A_d, E_d] = c2d(A, E, T_s);

% Problem b
sim('P5b.slx');
measurement_noise_variance = var(compass5b.data);

% Problem c
Q = [30 0; 0 10 ^(-6)];
P_0_pri = [1 0 0 0 0; 0 0.013 0 0 0; 0 0 pi^2 0 0; 0 0 0 1 0; 0 0 0 0 2.5*10^-3];
R = measurement_noise_variance / T_s;
x_0 = [zeros(10, 1); P_0_pri(:)];
data = struct('A', A_d, 'B', B_d, 'C', C, 'E', E_d, 'R', R, 'Q', Q, 'x_0', x_0);

% Problem d
sim('P5d.slx');

%% Plots
figure;
plot(compass5d);
title(['Compass angle using feed forward with measurements noise and current'],'Interpreter', 'latex');
legend('Reference', 'Compass angle','Estimated compass angle', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(rudder_input5d);
title(['Rudder input using feed forward with measurements noise and current'],'Interpreter', 'latex');
legend('Rudder-input', 'Estimated current bias', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;