close all;
figure;
plot(compass5b);
title('Measurement noise', 'Interpreter', 'latex');
legend('Measurement noise','Interpreter', 'latex', 'Location', 'best');
xlabel('Time [ms]','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;