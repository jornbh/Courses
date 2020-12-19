close all;

figure;
plot(compass1d);
title('Step response','Interpreter', 'latex');
legend({'Model of system', 'Actual system'},'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;