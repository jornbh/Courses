close all;

figure;
plot(compass3d);
title(['Compass angle from PD-regulator with disturbances in measurements and waves'],'Interpreter', 'latex');
legend('Reference', 'Compass angle', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(rudder_input3d);
title(['Rudder input from PD-regulator with disturbances in measurements and waves'],'Interpreter', 'latex');
legend('Rudder-input', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;