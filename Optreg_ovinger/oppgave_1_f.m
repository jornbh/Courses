run oppgave_1_d


lower_bound =-1;
upper_bound =1;   

lb = [ -Inf*[ones([N*size_of_x, 1]) ] ; [lower_bound*ones([N*size_of_u,1])] ]
ub  = [ Inf*[ones([N*size_of_x, 1]) ] ; [upper_bound*ones([N*size_of_u,1])] ];
OtherSolution =  quadprog( G_eq, [],  [], [] , A_eq, b_eq, lb, ub);
u_out= OtherSolution( N*size_of_x:1:N*(size_of_x + size_of_u));
hold on 
plot( 1:N, OtherSolution(3:size_of_x:N*size_of_x), '--' )
plot(1:N+1, OtherSolution( N*size_of_x:1:N*(size_of_x + size_of_u))); 
hold off;