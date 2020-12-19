function [integrator_plant, reference_B] = add_integrator_to_plant(plant)
% add_integrator_to_plant - Add integrator ouputs to plant
%
% Syntax: [integrator_plant, reference_B] = add_integrator_to_plant(plant)
% 
% 
% Adds states to a contineous plant that are the integral of the normal outputs,
% It also expands the outputs with the new integral states
% 
% Parameters: 
%       plant: A contineous-time state space system
%
% Return-values:
%
%   integrator_plant: The ecpanded plant (contineus time state-space model)
%   reference_B: Vector with access to the integrator states
%


%% Define some shorthand for the function
N_outputs = size(plant.C,1); 
N_inputs = size(plant.B,2); 
N_states = size(plant.A,1);

A = plant.A;
B = plant.B;
C = plant.C; 
D = plant.D; 

% Filler matrices
zeros_oo = zeros([N_outputs, N_outputs]); % output-> output
zeros_so = zeros([N_states, N_outputs]);  % output -> state
zeros_oi = zeros([N_outputs,N_inputs]);  % output-> inputs


%% The expanded system with integrated states is suposed to be on the form: 
%                           [        ]               [      ]
%                           [  A  0  ]               [  B   ]
%       x_expanded_dot =    [  C  0  ] * x_expanded  [  0   ] * u
%                           [        ]               [      ]
%        
%                           [        ]               [      ]                
%                           [  C 0   ]               [  D   ]        
%       y_expanded =        [  0 I   ] * x_expanded  [  0   ]*u
%                           [        ]               [      ]    

expanded_A = [
                [A, zeros_so];
                [C, zeros_oo]
            ];


expanded_B = [
                B; 
                zeros_oi
            ];

expanded_C = blkdiag( C, eye(N_outputs)); 

expanded_D = [D; zeros(N_outputs, N_inputs)];



% Return values: 
integrator_plant = ss(              ...
                        expanded_A, ...
                        expanded_B, ...
                        expanded_C, ...
                        expanded_D  ...
                    );

% A vector for setting a reference value the integrator has to follow                    
reference_B = [
                    zeros(N_states, N_outputs); 
                    eye(N_outputs)
                ];

    
end
