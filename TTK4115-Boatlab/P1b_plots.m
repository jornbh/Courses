close all;

figure;
plot(compass1b_1);
title(['Compass angle without disturbances, frequency = ', num2str(omega_1)],'Interpreter', 'latex');
legend('Compass angle','Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(compass1b_2);
title(['Compass angle without disturbances, frequency = ', num2str(omega_2)],'Interpreter', 'latex');
legend('Compass angle','Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;