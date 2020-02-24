run init04


% x = [ lambda, r, p, p_dot, e, e_dot]

% Vi vil ha inequality constraints på e_dot og lambda_dot


r_max =10;
e_dot_max = 10;
mu =2; 
N = 30; 

% A_ieq0 = [  0   0   0   0   0   0   
%             0   0   0   0   0   0                                                              
%             0   1   0   0   0   0                                                              
%             0   0   0   0   0   0                                                             
%             0   0   0   0   0   0                                                                
%             0   0   0   0   0   1                                                                    
% ];


% b_ieq0 = [      0;
%               r_max;
%                 0;
%                 0;
%                 0;
%             e_dot_max 
% ];

A_ieq_other= [   0  1   0   0   0   0;
                 0  0   0   0   0   1          
]
u_limit_other = zeros(2, mu),
b_ieq_other = [  r_max;                                
                 e_dot_max                               
]
u_ieq0 = zeros([mu,mu]); 


% b_ieq = kron( ones(N,1), b_ieq0);
% A_ieq = [ kron( eye(N), A_ieq0)    kron(eye(N), u_limit_other)]; 
% 



%% Input blocking ???
N = 30 
input_intervals = [1]; 
total =1; 
multiplier = 1
while total< N
    input_intervals = [ input_intervals multiplier];
    total = total + multiplier;
    multiplier= multiplier*2;

end;
input_intervals(end) = input_intervals(end)- total + N;
input_intervals = sort(input_intervals); 

%% Making an input-blocked matrix 
collumns=[]
last = 1;
for i = input_intervals
    col = zeros(1, N);
    col(last:i) = ones(1, i-last); 
    last =last + i; 
    collumns = [ collumns col];
end; 


