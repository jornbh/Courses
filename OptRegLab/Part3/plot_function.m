close all;

load('lambda_practical.mat')
load('p_practical.mat')
load('p_dot_practical.mat')
load('p_ref_practical.mat')
load('r_practical.mat')


figure(10); hold on;
subplot(5,1,1); hold on;
plot(p_ref(1,1:end), p_ref(2, 1:end), '-r');
plot(t,u,'-b'); 
legend({'$u$', '$u_k$'}, 'Interpreter', 'latex', 'FontSize', 15, 'Location', 'best');
xlim([0 35]);
ylabel('$u$', 'Interpreter', 'latex');
grid on;

subplot(5,1,2); hold on;
plot(lambda(1,1:end), lambda(2, 1:end), 'r');
plot(t,x1,'b');
legend({'$\lambda$', '$\lambda_{ref}$'}, 'Interpreter', 'latex');
xlim([0 35]);
ylabel('$\lambda$', 'Interpreter', 'latex');
grid on;

subplot(5,1,3); hold on;
plot(r(1,1:end), r(2, 1:end), 'r');
plot(t,x2,'-b');
legend({'$r$', '$r_{ref}$'}, 'Interpreter', 'latex');
xlim([0 35]);
ylabel('$r$', 'Interpreter', 'latex');
grid on;

subplot(5,1,4); hold on;
plot(p(1,1:end), p(2, 1:end), 'r');
plot(t,x3,'-b');
legend({'$p$', '$p_{ref}$'}, 'Interpreter', 'latex');
xlim([0 35]);
ylabel('$p$', 'Interpreter', 'latex');
grid on;

subplot(5,1,5); hold on; 
plot(p_dot(1,1:end), p_dot(2, 1:end), 'r');
plot(t,x4,'-b'),
legend({'$\dot{p}$', '$\dot{p}_{ref}$'}, 'Interpreter', 'latex');
xlim([0 35]);
ylabel('$\dot{p}$', 'Interpreter', 'latex');
xlabel('time (s)');
grid on;