clear;
close all;
load('wave.mat');
%% Parameters
omega_1 = 0.005;
omega_2 = 0.05;

A_1 = 29.3582;
A_2 = 0.8491;

T = sqrt(((A_2 * omega_2)^2 - (A_1 * omega_1)^2)/(A_1^2*omega_1^4 - A_2^2 * omega_2^4));
K = A_1 * omega_1 * sqrt(1 + omega_1 ^2 * ((A_2 * omega_2)^2 - (A_1 * omega_1)^2)/(A_1^2*omega_1^4 - A_2^2 * omega_2^4));

%% system tf
s = tf('s');
h_sys = (K)/(s*(1+T*s));

%% PD controller
w_c = 0.1;
Pm = 50;
T_d = T;

T_f = tan(deg2rad(90 - Pm)) / w_c;
K_pd = w_c*sqrt(T_f^2*w_c^2 + 1)/ K;
h_pd = K_pd*(1 + T_d*s)/(1+ T_f*s);
sys_pd = h_sys* h_pd;
