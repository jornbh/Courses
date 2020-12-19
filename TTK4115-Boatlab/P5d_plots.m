close all;
figure;
plot(compass5d);
title(['Compass angle using feed forward with measurements noise and current'],'Interpreter', 'latex');
legend('Reference', 'Compass angle','Estimated compass angle', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;

figure;
plot(rudder_input5d);
title(['Rudder input using feed forward with measurements noise and current'],'Interpreter', 'latex');
legend('Rudder-input', 'Estimated current bias', 'Interpreter', 'latex', 'Location', 'best');
xlabel('Time','Interpreter', 'latex');
ylabel('Angle [deg]','Interpreter', 'latex');
grid on;