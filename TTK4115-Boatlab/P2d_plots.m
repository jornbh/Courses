close all;

plot(omega, S_w)
hold on
plot(omega, S_w_analytic(lambda, omega));
title('Power spectral density function', 'Interpreter', 'latex');
legend({'Estimated PSD, $S_{\psi_\omega}(\omega)$', 'Analytic PSD, $P_{\psi_\omega}(\omega)$'},'Interpreter', 'latex', 'Location', 'best');
xlabel('Frequency [rad/s]','Interpreter', 'latex');
ylabel('Power per frequency [W s/rad]','Interpreter', 'latex');
grid on;
xlim([0, pi/2]);