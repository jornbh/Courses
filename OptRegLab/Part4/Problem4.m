%% Initialization and model definition

%init;
run init;
global N mx alpha beta lambda_t

A_cont = [0      1        0           0           0            0;
          0      0       K_2          0           0            0; 
          0      0        0           1           0            0;
          0      0    -K_1*K_pp   -K_1*K_pd       0            0;
          0      0        0           0           0            1;
          0      0        0           0       -K_3*K_ep    -K_3*K_ed];

% u = [P_c, E_c]'      
B_cont = [   0            0; 
             0            0;
             0            0;
          K_1*K_pp        0;
             0            0;
             0        K_3*K_ep]; 

% Number of states and inputs
mx = size(A_cont,2); % Number of states (number of columns in A)
mu = size(B_cont,2); % Number of inputs(number of columns in B)

% Initial values
x1_0 = pi;                                  % Lambda
x2_0 = 0;                                   % r
x3_0 = 0;                                   % p
x4_0 = 0;                                   % p_dot
x5_0 = 0;                                   % e
x6_0 = 0;                                   % e_dot
x0 = [x1_0 x2_0 x3_0 x4_0, x5_0, x6_0]';    % Initial values

% Time horizon and initialization
N  = 40;                                    % Time horizon for states
M  = N;                                     % Time horizon for inputs
z  = zeros(N * mx + M * mu, 1);             % Initialize z for the whole horizon
z0 = z;
z0(1:mx) = x0;

% Bounds
bound = 30 * pi / 180;
ul 	    = [-bound; -inf];                  % Lower bound on control
uu 	    = [bound; inf];                    % Upper bound on control

xl      = -Inf * ones(mx,1);               % Lower bound on states (no bound)
xu      =  Inf * ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = -bound;                          % Lower bound on state x3
xu(3)   = bound;                           % Upper bound on state x3

% Generate constraints on measurements and inputs
[vlb,vub]       = gen_constraints(N,M,xl,xu,ul,uu);  % hint: gen_constraints
vlb(N*mx+M*mu)  = 0;                                 % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                                 % We want the last input to be zero

% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = diag([1 0 0 0 0 0]);
P1 = diag([2 ; 0.1]);                      % Weight on input
Q = gen_q(Q1,P1,N,M);                  % Generate Q, hint: gen_q
Q_size = size(Q);
c = zeros(Q_size(1), 1);               % Generate c, this is the linear constant term in the QP

%% Discretization
delta_t = 0.25;
A = (eye(size(A_cont, 1)) + A_cont * delta_t);
B = B_cont * delta_t;

%% Generate system matrixes for linear model
Aeq = gen_aeq(A, B, N, mx, mu);               % Generate A, hint: gen_aeq
%x1 = A_cont * x0;
beq = zeros(N * mx, 1);
value = A * x0;
beq(1) = value(1);                            % Generate b


%% Optimization
alpha = 0.2;
beta = 20;
lambda_t = 2 * pi / 3;
fun = @(x) x'*Q*x;

opt = optimoptions('fmincon', 'Algorithm', 'sqp');
z = fmincon(fun, z0,[],[],Aeq,beq,vlb,vub, @nonlcon_fun, opt);


%% Extract control inputs and states
u1  = [z(N*mx+1 : mu :  N*mx+M*mu); z(N*mx+M*mu-1)];       % Control input from solution
u2  = [z(N*mx+2 : mu :  N*mx+M*mu); z(N*mx+M*mu)];

x1 = [x0(1); z(1 : mx : N*mx)];                   % State x1 from solution
x2 = [x0(2); z(2 : mx : N*mx)];                   % State x2 from solution
x3 = [x0(3); z(3 : mx : N*mx)];                   % State x3 from solution
x4 = [x0(4); z(4 : mx : N*mx)];                   % State x4 from solution
x5 = [x0(5); z(5 : mx : N*mx)];                   % State x5 from solution
x6 = [x0(6); z(6 : mx : N*mx)];                   % State x6 from solution

num_variables = 5 / delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

u1  = [zero_padding; u1; zero_padding];
u2  = [zero_padding; u2; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];

%% Plotting
t = 0 : delta_t : delta_t*(length(u1)-1);

figure(3);
subplot(811); grid on;
stairs(t,u1), grid
ylabel('u_1');
subplot(812); grid on;
stairs(t,u2), grid
ylabel('u_2');
subplot(813)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')
subplot(814)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')
subplot(815)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')
subplot(816)
plot(t,x4,'m',t,x4','mo'),grid
ylabel('p_{dot}')
subplot(817)
plot(t,x5,'m',t,x5,'mo'),grid
ylabel('e')
subplot(818)
plot(t,x6,'m',t,x6','mo'),grid
xlabel('time (s)'),ylabel('e_{dot}')

%% Making a K-matrix

% Lambda r p p_dot 
x1_star = [t; x1'];
x2_star = [t; x2'];
x3_star = [t; x3'];
x4_star = [t; x4'];

x_opt = [x1 x2 x3 x4 x5 x6];
u_opt = [u1 u2];

u_star = timeseries(u_opt, t); 
x_star = timeseries(x_opt, t);

Q_k = diag([5 0.1 0.2 0 200 10]);
R_k = diag([2 2]);
K = dlqr(A, B, Q_k, R_k);


