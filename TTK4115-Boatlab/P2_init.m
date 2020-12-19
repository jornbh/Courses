run('common.m');

%% Problem a
% Estimated psd
[S_w, f] = pwelch(deg2rad(psi_w(2,:)), 4096, [], [], 10);
S_w = S_w ./ (2*pi);
omega = (2* pi) .* f;
plot(omega, S_w)
hold on

%% Problem c
w_0 = omega(find(S_w == max(S_w)))

%d)
sigma = sqrt(max(S_w));
S_w_analytic = @(lambda, omega) (2*lambda.*w_0.*omega*sigma).^2 ./ ...
    ((w_0.^2 - omega.^2).^2 +(2.*lambda.*w_0.*omega).^2)
lambda_0 = 0.1;
lambda = lsqcurvefit(S_w_analytic, lambda_0, omega, S_w)
plot(omega, S_w_analytic(lambda, omega));

title('Power spectral density function', 'Interpreter', 'latex');
legend({'Estimated PSD, $S_{\psi_\omega}(\omega)$', 'Analytic PSD, $P_{\psi_\omega}(\omega)$'},'Interpreter', 'latex', 'Location', 'best');
xlabel('Frequency [rad/s]','Interpreter', 'latex');
ylabel('Power per frequency [W s/rad]','Interpreter', 'latex');
grid on;
xlim([0, pi/2]);