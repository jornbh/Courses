

A = [0,0,0;
    0,0,1;
    0.1 , -0.79, 1.78
    ];
    
B = [1;0;0.1];
C= [0, 0,1]; 

G = diag([0,0,1]);
R = 1; %Endre for å forandre regulatoren
x_0 = [0;0;1]; 
N =30; 
size_of_x =(size(x_0));
size_of_x = size_of_x(1);
size_of_u = 1;
b_eq = zeros([N*(size_of_x), 1]);
b_eq(1:size_of_x) = A*x_0; 
A_eq = [kron(diag(  ones( [(N-1),1] ),-1), -A) + eye(size_of_x*N)  kron(eye(N), -B) ]; 

G_eq = [kron( eye(N), G )   , zeros([N*size_of_x, N* size_of_u]) ;
        zeros( N* [size_of_u, size_of_x] ) kron( eye(N), R) ];
    
 KKT_matrix = [ G_eq, -A_eq';
                 A_eq, zeros(N*(size_of_x))];
SOLUTION =  linsolve(KKT_matrix, [zeros([N*(size_of_x+size_of_u),1]); b_eq]);
hold on
% plot( 1:N, SOLUTION(1:size_of_x:N*size_of_x) )
% plot( 1:N, SOLUTION(2:size_of_x:N*size_of_x) )
plot( 1:N, SOLUTION(3:size_of_x:N*size_of_x) );

% plot(1:N+1, SOLUTION( N*size_of_x:1:N*(size_of_x + size_of_u))); 
hold off
