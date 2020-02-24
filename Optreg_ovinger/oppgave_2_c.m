
% Exact copy of last task, but A_real and B_real are different
close all;
Output_list = zeros([3,30]); 
inputs = zeros([1, 30]); 
x_0 = [0;0;1]; 

A = [0,0,0;
    0,0,1;
    0.1 , -0.79, 1.78
    ];
    
B = [1;0;0.1];
C= [0, 0,1]; 

G = diag([1,1,1]);
R = 1; %Endre for å forandre regulatoren
% Originalt uttrykk for x_0 = [0;0;1];  fjernet og flyttet utenfor løkken 
N =30; 
size_of_x =(size(x_0));
size_of_x = size_of_x(1);
size_of_u = 1;
b_eq = zeros([N*(size_of_x), 1]);
A_real = [0 0 0;
            0 0 1;
            0.1 -0.85 1.85];
B_real = [1;0;0];
for itterator_timestep = 1:N
%% Kopi av 1_d, men med N = 30, og G = cGC^T [1,0,0; 0,1,0; 0,0,1]





b_eq(1:size_of_x) = A*x_0; 
A_eq = [kron(diag(  ones( [(N-1),1] ),-1), -A) + eye(size_of_x*N)  kron(eye(N), -B) ]; 

G_eq = [kron( eye(N), G )   , zeros([N*size_of_x, N* size_of_u]) ;
        zeros( N* [size_of_u, size_of_x] ) kron( eye(N), R) ];
    
KKT_matrix = [ G_eq, -A_eq';
                 A_eq, zeros(N*(size_of_x))];
SOLUTION =  linsolve(KKT_matrix, [zeros([N*(size_of_x+size_of_u),1]); b_eq]);



%% Slutt på kopi

lower_bound =-1;
upper_bound =1;   

lb = [ -Inf*[ones([N*size_of_x, 1]) ] ; [lower_bound*ones([N*size_of_u,1])] ];
ub  = [ Inf*[ones([N*size_of_x, 1]) ] ; [upper_bound*ones([N*size_of_u,1])] ];
N_2b =N;
Solution_of_weighting_all_states =  quadprog( G_eq, [],  [], [] , A_eq, b_eq, lb, ub);

u_0 = Solution_of_weighting_all_states(N*size_of_x +1); % The first input in the timeseries
Output_list(itterator_timestep, 1:3) = x_0; 
x_0 = A_real*x_0 + B_real*u_0; % Setting the state for the next itteration 
inputs(itterator_timestep) = u_0; 
if itterator_timestep ==1
   Original_estimate = Solution_of_weighting_all_states;
end
end;
figure(5)
hold on 
plot( 1:N, Original_estimate(1:size_of_x:N_2b*size_of_x) );%State 1
plot( 1:N, Original_estimate(2:size_of_x:N_2b*size_of_x) );%State 2
plot( 1:N, Original_estimate(3:size_of_x:N_2b*size_of_x) )%State 3
plot(1:N_2b+1, Original_estimate( N_2b*size_of_x:1:N_2b*(size_of_x + size_of_u))); 
hold off
plot_outputs= Output_list'; 
figure(6)
hold on
plot(1:N, inputs, '--' )
%plot(1:N, Output_list(1:size_of_x:N_2b*size_of_x)); 
plot(1:N, Output_list)
hold off;


