a = 0.2;
b = 20;
fun = @(t) a*exp(-b*(t-lambda_t)^2);


%Sær Marius triksing. Ikke se for nøye etter på denne :)

figure(1); clf(1);grid on; hold on;
    set(gca,'Xdir','reverse')
    fplot(@(x) fun(x), 'linewidt', 2);
    stairs(x1_star(2, :), x5_star(2,:), '-om', 'linewidt', 1.3, 'MarkerSize',4);
    
    axis([-0.5 3.2 0 0.205]);
    %lgd = legend('Constraint', 'Open loop simulation');
    %lgd.FontSize = 14;
    %xlabel('$\lambda$ [rad]', 'Interpreter', 'latex', 'fontSize', 20);
    %ylabel('$e$ [rad]', 'Interpreter', 'latex', 'fontSize', 20);

%%    
figure(1); grid on; hold on;
    set(gca,'Xdir','reverse')
    %fplot(@(x) fun(x), 'linewidt', 2);
    stairs(x1_star(2, :), x5_star(2,:), '-o', 'linewidt', 1.3, 'MarkerSize',4);
    
    axis([-0.5 3.2 0 0.205]);
    lgd = legend({'Constraint', 'N = 80, $\Delta t$ = 0.13', 'N = 40, $\Delta t$ = 0.25'} , 'Interpreter', 'latex');
    lgd.FontSize = 16;
    xlabel('$\lambda$ [rad]', 'Interpreter', 'latex', 'fontSize', 20);
    ylabel('$e$ [rad]', 'Interpreter', 'latex', 'fontSize', 20);
    