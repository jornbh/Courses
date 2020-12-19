clear;
close all;
load('wave.mat');
%% Part 1
omega_1 = 0.005;
omega_2 = 0.05;

% Problem b
sim('P1b_1.slx');
A_1 = (max(compass1b_1.data) - min(compass1b_1.data(1000:end,1))) / 2;

sim('P1b_2.slx');
A_2 = (max(compass1b_2.data) - min(compass1b_2.data(1000:end,1))) / 2;

T = sqrt(((A_2 * omega_2)^2 - (A_1 * omega_1)^2)/(A_1^2*omega_1^4 - A_2^2 * omega_2^4));
K = A_1 * omega_1 * sqrt(1 + omega_1 ^2 * ((A_2 * omega_2)^2 - (A_1 * omega_1)^2)/(A_1^2*omega_1^4 - A_2^2 * omega_2^4));

s = tf('s');
h_sys = (K)/(s*(1+T*s));

% Problem c
sim('P1c_1.slx');
sim('P1c_2.slx');

% Problem d
sim('P1d.slx');
difference = compass1d.data(end) - compass1d.data(end/2);
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

% Problem b
sim('P3b.slx');

%% Plots
figure;
plot(compass3b);
title(['Compass angle from PD-regulator with disturbances in measurements'],'Interpreter', 'latex');
legend('Reference', 'Compass angle', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(rudder_input3b);
title(['Rudder input from PD-regulator with disturbances in measurements'],'Interpreter', 'latex');
legend('Rudder-input', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;