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

%% Problem a
A = [0 1 0 0 0; -w_0^2 -2*lambda*2 0 0 0; 0 0 0 1 0; 0 0 0 -1/T -K/T; 0 0 0 0 0];
B = [0 0 0 K/T 0];
C = [0 1 1 0 0];
E = [0 0; K_w 0; 0 0; 0 0; 0 1];

%4b)
A_b = [0 1; 0 -1/T];
C_b = [1 0];
rank(obsv(A_b,C_b));
%observable

%4c)

A_c = [0 1 0; 0 -1/T -K/T;0 0 0];
C_c = [1 0 0];
rank(obsv(A_c,C_c));
%observable

%4d)
A_d = [0 1 0 0; 
    -w_0^2 -2*lambda*w_0 0 0;  
    0 0 0 1;
    0 0 0 -1/T];
C_d = [0 1 1 0];
rank(obsv(A_d,C_d));
% observable    

%4e) observable as shit
A_e =[0 1 0 0 0;
     -w_0^2 -2*lambda*w_0 0 0 0;
      0 0 0 1 0;
      0 0 0 -1/T -K/T;
      0 0 0 0 0];
C_e = [0 1 1 0 0 ];

(obsv(A_e,C_e));


