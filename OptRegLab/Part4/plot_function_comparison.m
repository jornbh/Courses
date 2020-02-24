%close(10);

%parameters
end_time = 20;

load('lambda_practical.mat')
load('p_practical.mat')
load('p_dot_practical.mat')
load('e_practical.mat')
load('e_dot_practical.mat')
load('p_ref_practical.mat')
load('e_ref_practical.mat')
load('r_practical.mat')


figure(10); clf(10); hold on;

subplot(811); hold on;
plot(p_ref(1,1:end), p_ref(2, 1:end), '-m');
plot(t,u1,'-b');
legend({'$p_{c}$', '$p_{c,ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-0.8 0.8]);
ylabel('$p_c$', 'Interpreter', 'latex');
grid on;

subplot(812); hold on;
plot(e_ref(1,1:end), e_ref(2, 1:end), '-m');
plot(t,u2,'-b'); 

legend({'$e_c$','$e_{c, ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-0.5 0.5]);
ylabel('$e_c$', 'Interpreter', 'latex');
grid on;

subplot(813); hold on;
plot(lambda(1,1:end), lambda(2, 1:end), 'm');
plot(t,x1,'b');
legend({'$\lambda$', '$\lambda_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-2 4]);
ylabel('$\lambda$', 'Interpreter', 'latex');
grid on;

subplot(814); hold on;
plot(r(1,1:end), r(2, 1:end), 'm');
plot(t,x2,'-b');
legend({'$r$', '$r_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-1 0.2]);
ylabel('$r$', 'Interpreter', 'latex');
grid on;

subplot(815); hold on;
plot(p(1,1:end), p(2, 1:end), 'm');
plot(t,x3,'-b');
legend({'$p$', '$p_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-0.5 0.5]);
ylabel('$p$', 'Interpreter', 'latex');
grid on;

subplot(816); hold on; 
plot(p_dot(1,1:end), p_dot(2, 1:end), 'm');
plot(t,x4,'-b'),
legend({'$\dot{p}$', '$\dot{p}_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-0.6 0.6]);
ylabel('$\dot{p}$', 'Interpreter', 'latex');
grid on;

subplot(817); hold on; 
plot(e(1,1:end), e(2, 1:end), 'm');
plot(t,x5,'-b'),
legend({'$e$', '$e_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([0 0.2]);
ylabel('$e$', 'Interpreter', 'latex');
grid on;

subplot(818); hold on; 
plot(e_dot(1,1:end), e_dot(2, 1:end), 'm');
plot(t,x6,'-b'),
legend({'$\dot{e}$', '$\dot{e}_{ref}$'}, 'Interpreter', 'latex');
xlim([0 end_time]);
ylim([-0.1 0.1]);
ylabel('$\dot{e}$', 'Interpreter', 'latex');
xlabel('time (s)');
grid on;