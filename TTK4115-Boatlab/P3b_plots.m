close all;

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