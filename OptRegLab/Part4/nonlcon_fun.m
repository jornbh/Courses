function [C_ieq, C_eq] = nonlcon_fun(z)
    global N mx alpha beta lambda_t
    C_ieq = alpha * exp(- beta*(z(1 : mx : N*mx) - lambda_t).^2 ) - z(5 : mx : N*mx);
    C_eq = [];
end

%     C_ieq = zeros(N, 1);
%     for k = 1 : N
%             C_ieq(k) = alpha * exp(-beta * (z(1 + (k - 1) * mx) - lambda_t)^2) - z(5 + (k - 1) * mx);
%     end