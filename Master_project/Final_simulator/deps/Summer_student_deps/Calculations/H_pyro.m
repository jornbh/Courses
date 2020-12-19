function H_pyro = H_pyro
% This function calculates the pyrolysis heat, from the HHV of waste and
% the heat of combustion of char burning and pyrolysis gas burning

Mw_w    = [12.011;         % C
           1.0079;         % H
           32.06;          % S
           35.45;          % Cl 
           14.007;         % N   
           15.999];        % O

% Combustion of 1 kg of waste
M_w     = [26;          % Moisture (H2O)
           20.8; 
           3; 
           0.3; 
           0.2; 
           0.4; 
           17.5; 
           23.8]./100;  % Ashes
M_w     = M_w./sum(M_w);       
%% WATER
DeltaH_H2O  = -2.26E+06;                % [J/kg]
H_water = DeltaH_H2O.*M_w(1);           % [J]

%% CHAR PRODUCTION
M_comb 	= M_w(2:end-1)        
N_comb  = M_comb./Mw_w;
% Volatiles/fixed carbon ratio
V_F_ratio   = 32.00/8.20;
% Char mass produced [kg]
M_char      = M_comb(1)./(1+V_F_ratio) % [kg]
DeltaH_Char = 110.63E+6;                % J/kmol
DeltaH_Char = DeltaH_Char./Mw_w(1)     % [J/kg]
% Heat from combustion of char
H_char      = M_char.*DeltaH_Char      % [J]

N_comb(1)   = N_comb(1) - M_char./Mw_w(1);

%% PYROLYSIS GAS PRODUCTION
% Molar flows of pyrolysis gas components  
options     = optimoptions('lsqnonlin','Display','off');
Ni_gas      =  lsqnonlin(@Gas_fluxes,zeros(6,1),zeros(6,1),[],options); % [kmol]
DeltaH_comb = [802320E+3;               % [J/kmol]
               242000E+3;
               283150E+3];
Ni_comb = [Ni_gas(3);
           Ni_gas(4);
           Ni_gas(2)*2];
    
% Heat from combustion of pyrolysis gases
H_comb  = sum(Ni_comb.*DeltaH_comb)    % [J]


H_tot   = H_water + H_comb + H_char;
HHV     = 12.74E+6;
% Pyrolysis heat is the difference between the waste HHv and the heat
% realeased by char and gas combustion
H_pyro  = HHV-H_tot

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Res = Gas_fluxes(x)
S  = [0 0 0 0 2 0;      % N2
      3 0 0 0 0 4;      % C3O24
      1 4 0 0 0 0;      % CH4
      0 2 0 0 0 0;      % H2
      0 0 1 0 0 2;      % H2S
      0 1 0 1 0 0];    % HCl
Res = S'*x - N_comb;
end
end