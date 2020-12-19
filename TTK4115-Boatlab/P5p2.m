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

%% Plots
plot(omega, S_w)
hold on
plot(omega, S_w_analytic(lambda, omega));
title('Power spectral density function', 'Interpreter', 'latex');
legend({'Estimated PSD, $S_{\psi_\omega}(\omega)$', 'Analytic PSD, $P_{\psi_\omega}(\omega)$'},'Interpreter', 'latex', 'Location', 'best');
xlabel('Frequency [rad/s]','Interpreter', 'latex');
ylabel('Power per frequency [W s/rad]','Interpreter', 'latex');
grid on;
xlim([0, pi/2]);