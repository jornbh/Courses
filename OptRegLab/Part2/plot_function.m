load('lambda_practical.mat')
load('p_practical.mat')
load('p_dot_practical.mat')
load('p_ref_practical.mat')
load('r_practical.mat')


figure(10); hold on;
subplot(5,1,1);
plot(p_ref(1,1:end), p_ref(2, 1:end), '-');
ylabel('$u$', 'Interpreter', 'latex');
grid on;

subplot(5,1,2);
plot(lambda(1,1:end), lambda(2, 1:end), 'm');
ylabel('$\lambda$', 'Interpreter', 'latex');
grid on;

subplot(5,1,3);
plot(r(1,1:end), r(2, 1:end), 'm');
ylabel('$r$', 'Interpreter', 'latex');
grid on;

subplot(5,1,4);
plot(p(1,1:end), p(2, 1:end), 'm');
ylabel('$p$', 'Interpreter', 'latex');
grid on;

subplot(5,1,5);
plot(p_dot(1,1:end), p_dot(2, 1:end), 'm');
ylabel('$\dot{p}$', 'Interpreter', 'latex');
xlabel('time (s)');
grid on;