function R = CALC_Fuel_NOx(T_w,M_w)
%% Calculation of the NOx production from fuel bound nitrogen
% The production of NOx from the fuel is a function of the temperature in
% each cell, the amount of present oxygen in the air and fuel-bound
% nitrogen

S  = [0 0 0 0 (2+x) 2*x 0;% N(2+x)0(2x)
      3 0 0 0 0 4 0;      % C3O4
      1 4 0 0 0 0 0;      % CH4
      0 2 0 0 0 0 0;      % H2
      0 0 1 0 0 2 0;      % SO2
      0 1 0 1 0 0 0;      % HCl
      0 1 0 0 0 0 1]';    % HF
  
[Ni_gas(1);       % N2
                      Ni_gas(2);       % CO2 (1/3 of C3/2O2)
                      0;               % O2
                      Ni_gas(3);       % CH4 
                      Ni_gas(4);       % H2
                      Ni_gas(2)*2;     % CO  (2/3 of C3/2O2)
                      Ni_gas(5);       % H2S
                      Ni_gas(6);       % HCl   
                      Ni_gas(7);       % HF
                      x.*Ni_gas(1)];   % NOx
                  
k.Mw_w      = [12.011;         % C
               1.0079;         % H
               32.06;          % S
               35.45;          % Cl 
               14.007;         % N   
               15.999;         % O
               18.998];        % F