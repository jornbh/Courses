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

%% Plots
figure;
plot(compass1d);
title('Step response','Interpreter', 'latex');
legend({'Model of system', 'Actual system'},'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;