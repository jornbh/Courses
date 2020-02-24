close all;

%parameters
end_time = 30;

load('lambda_practical.mat')
load('p_practical.mat')
load('p_dot_practical.mat')
load('p_ref_practical.mat')
load('r_practical.mat')


figure(11); hold on;

subplot(511); hold on;
plot(p_ref(1,1:end), p_ref(2, 1:end), '-m');
plot(t,u,'-b'); 
legend({'$u$', '$u_k$',}, 'Interpreter', 'latex', 'FontSize', 15);
xlim([0 end_time]);
ylabel('$u$', 'Interpreter', 'latex');
grid on;

subplot(512); hold on;
plot(lambda(1,1:end), lambda(2, 1:end), 'm');
plot(t,x1,'b');
legend({'$\lambda$', '$\lambda_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylabel('$\lambda$', 'Interpreter', 'latex');
grid on;

subplot(513); hold on;
plot(r(1,1:end), r(2, 1:end), 'm');
plot(t,x2,'-b');
legend({'$r$', '$r_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylabel('$r$', 'Interpreter', 'latex');
grid on;

subplot(514); hold on;
plot(p(1,1:end), p(2, 1:end), 'm');
plot(t,x3,'-b');
legend({'$p$', '$p_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylabel('$p$', 'Interpreter', 'latex');
grid on;

subplot(515); hold on; 
plot(p_dot(1,1:end), p_dot(2, 1:end), 'm');
plot(t,x4,'-b'),
legend({'$\dot{p}$', '$\dot{p}_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylabel('$\dot{p}$', 'Interpreter', 'latex');
grid on;