close all;
figure;
plot(compass5e);
title(['Compass angle using state feedback with estimate, measurements noise, current and waves'],'Interpreter', 'latex');
legend('Reference', 'Compass angle','Estimated compass angle', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(rudder_input5e);
title(['Rudder input using state feedback with estimate, measurements noise, current and waves'],'Interpreter', 'latex');
legend('Rudder-input', 'Estimated current bias', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;