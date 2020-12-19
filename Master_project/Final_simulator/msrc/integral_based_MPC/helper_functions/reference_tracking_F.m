function [F_x_ref, F_u_ref] = reference_tracking_F(A, B,C, R,R_hat, Q_bar, Q_hat, N_prediction_steps, CONV)
    %Reference_tracking_F - Reference tracking costs

    %
    % Syntax: [F_x_ref, F_u_ref] = Reference_tracking_F(A,B,R,Q_bar)
    %
    % Takes in an MPC-system and a Q made to stabilize a plant, and returns the  f^T *x part that is needed to handle initial values and tracking references

    %% Cheapest stationary value tracking the reference
    U = eye(size(B, 2)); % All possible inputs
    X = (eye(size(A)) - A) \ (B * U); % All possible stationary values, given unit input
    Y = C * X; % All stationary outputs, given the input

    n_outputs = size(C, 1);

    % The best state has to be found by solving a QP
    A_eq = kron(eye(n_outputs), Y);
    b_eq = reshape(eye(n_outputs), [n_outputs^2, 1]);

    state_cost = (X' * Q_bar * X); % State cost, given an U

    H_stat_base = (state_cost + R); % Combined cost for a given input
    f_stat_base = zeros([1, size(B, 2)]); % This is suposed to be 0

    % Broadcast the costs to unit y
    H_stat = kron(eye(n_outputs), H_stat_base);
    f_stat = kron(ones([1, n_outputs]), f_stat_base);

    % Correct H to get proper symetry (Double precission messes up sometimes)
    H_stat = (H_stat + H_stat') ./ 2; 

    % Solve the QP to get the optimal stationary inputs
    u_star_flattened = quadprog(H_stat, f_stat, [], [], A_eq, b_eq);

    % The optimal solutions for a given reference output
    U_star = reshape(u_star_flattened, [size(B, 2), n_outputs]); % Make a matrix instead
    X_star = (eye(length(A)) - A) \ (B * U_star); % Cheapest states to get the wanted y

    %% Simulate a reference tracking controller

    y_star = [1; 0];

    y2x_star = kron(ones([N_prediction_steps, 1]), X_star);
    y2u_star = kron(ones([N_prediction_steps, 1]), U_star);
    ref = y2x_star * y_star;

    %% Extra matrices added to the cost functions
    F_x_ref = -CONV' * Q_hat * y2x_star;
    F_u_ref = -R_hat * y2u_star; % u*F_ref_u*u_ref



    
    %% The most normal solution is to simply limit the change in input instead of giving a cost to u

end
