close all;

figure;
plot(compass3c);
title(['Compass angle from PD-regulator with disturbances in measurements and current'],'Interpreter', 'latex');
legend('Reference', 'Compass angle', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(rudder_input3c);
title(['Rudder input from PD-regulator with disturbances in measurements and current'],'Interpreter', 'latex');
legend('Rudder-input', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;