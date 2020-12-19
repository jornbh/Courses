function [M, CONV] = make_MPC_response(disc_MPC_sys, N_prediction_steps)
%% Returns matrices giving exact solutions for a discrete system "disc_MPC_sys" over the next "N_prediction_steps" time-steps 
    

    % M: Exact solution of discrete x(t), given an initial condition x(0) and no input
        % [x(1)', x(2)',...,x(N_prediction_steps)']' = M*x(0)
    % CONV: Exact solutionof discrete x(t), given a series of inputs, and zero initial condition
        % [x(1)', x(2)',...,x(N_prediction_steps)'] = CONV*[u(1)', u(2)',...,u(N_prediction_steps)']'
        % CONV immitates "Σ A^(τ) *u(t - τ)", which is a discrete convolution
    % Linear systems allow for superposition 

    
    A = disc_MPC_sys.A;
    B = disc_MPC_sys.B;

    M = zeros([(N_prediction_steps) * size(A, 1), size(A, 1)]); % x(t) due to x(0)

    %% M = 
    % [       A                          ]
    % [       A^2                        ]  
    % [       ...                        ] 
    % [       A^(N_prediction_steps)     ]                      

    for i = 1:(N_prediction_steps)
        displacement               = (i - 1) * size(A, 1); 
        row_range                  = (1:size(A, 1)) + displacement;
        M(row_range, 1:size(A, 1)) = A^i;
    end


    %% CONV = 
    % [       B                          ,   0                          , ..., 0  ]
    % [       A^1*B                      ,   B                          , ..., 0  ]  
    % [       A^2*B                      ,   A*B                        , ..., 0  ]  
    % [       ...                        ,   ...                        , ..., 0  ] 
    % [       A^(N_prediction_steps-1)*B ,   A^(N_prediction_steps-2)*B , ..., B  ]                      

    CONV = zeros(size(B) * N_prediction_steps); % x(t) due to u(t)
    for block_row_no = 1:N_prediction_steps
        for block_col_no = 1:block_row_no
            % Chanced indecies  
            row_range = (block_row_no - 1) * size(B, 1) + (1:size(B, 1));
            col_range = (block_col_no - 1) * size(B, 2) + (1:size(B, 2));

            updated_block = A^(block_row_no - block_col_no) * B;
            CONV(row_range, col_range) = updated_block;
        end

    end

end
