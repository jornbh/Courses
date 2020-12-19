function R = CALC_Rw_Char(M_w,T_w,M_char,F_a,k_Mw_w,k_Mw_gas,k_p_amb)

R_ig    = 8.314462175*1E3; 

%% WATER
% Evaporation parameters from "Bryden, K.M., Ragland, K.W. and Rutland, C.J.,
% 2002. Modeling thermally thick pyrolysis of wood. Biomass and Bioenergy,
% 22(1), pp.41-53."
% Preexponential factors [1/s]
k_d_w   = 5.13E+8;        % Evaporation H2O
% Activation energy [J/kmol]
Ea_w    = 88E+06;          % Evaporation H2O

%% PYROLYSIS (data from Gorton and Knight, for pyrolysis of hardwood)
% Need better data here (maybe from GrateCFD?)
k_d_p   = 1.483e6;    
% Activation energy [J/kmol]
Ea_p    = 89.52E+06;    

%% REACTION RATES
% Water evaporation rate
R_w     = -k_d_w.*exp(-Ea_w./(R_ig.*T_w)).*M_w(1,:);
% Total pyrolysis rate
Mw_tot  = sum(M_w(2:end-1,:),1);
R_p     = -k_d_p.*exp(-Ea_p./(R_ig.*T_w)).*Mw_tot; % [kg/s]
% Inerts
R_inert = -0.1./4800.*M_w(end,:); % HERE FIND A MODEL FOR R_inert AS A   
                                  % FUNCTION OF (??) 

%% WASTE CONSUMPTION 
% Mass-based
Ri_p    = zeros(length(k_Mw_w),length(T_w));
Ni_p    = zeros(size(Ri_p));
for j = 1:length(T_w)
    if Mw_tot(j) == 0
        Ri_p(:,j)   = 0.*k_Mw_w;
        Ni_p(:,j)   = Ri_p(:,j)./k_Mw_w;
    else
        w_comb      = M_w(2:end-1,j)./Mw_tot(j);
        Ri_p(:,j)   = R_p(j).*w_comb;
        Ni_p(:,j)   = Ri_p(:,j)./k_Mw_w;
    end
end

% Molar-based
R.waste_cons= [R_w;
               Ri_p;
               R_inert];
%% PYROLYSIS CHAR PRODUCTION 
% Volatiles/fixed carbon ratio
V_F_ratio   = 32.00/8.20;
R.char      = -Ri_p(1,:)./(1+V_F_ratio);
Ni_p(1,:)   =  Ni_p(1,:).*V_F_ratio./(1+V_F_ratio);
%% PYROLYSIS GAS PRODUCTION
x           = 0.1;        % HERE FIND A MODEL FOR x AS A FUNCTION OF  
                          % TEMPERATURE AND COMPOSITION 
                          
y           = 0.1;                          
S  = [0 0 0 0 (2+x) 2*x 0;% N(2+x+y)O(2x)H(3*y) --> N2 + NO2 + NH3
      3 0 0 0 0 4 0;      % C3O4 --> CO2 + 2*CO
      1 4 0 0 0 0 0;      % CH4
      0 2 0 0 0 0 0;      % H2
      0 0 1 0 0 2 0;      % SO2
      0 1 0 1 0 0 0;      % HCl
      0 1 0 0 0 0 1]';    % HF
 
% Molar flows of pyrolysis gas components  
Ni_gas_tot  = zeros(length(k_Mw_gas)-1,length(T_w));
Fi_gas      = zeros(size(Ni_gas_tot));

for j = 1:length(T_w)
    Ni_gas  = (S)\(-Ni_p(:,j));
    Ni_gas_tot(:,j)  = [Ni_gas(1);       % N2
                        Ni_gas(2);       % CO2 (1/3 of C3/2O2)
                        0;               % O2  --> no molecular oxygen is produced during pyrolysis
                        Ni_gas(3);       % CH4 
                        Ni_gas(4);       % H2
                        Ni_gas(2)*2;     % CO  (2/3 of C3/2O2)
                        Ni_gas(5);       % SO2
                        Ni_gas(6);       % HCl   
                        Ni_gas(7);       % HF
                        x.*Ni_gas(1) ;   % NO2
                        y.*Ni_gas(1)];   % NH3
    Fi_gas(:,j) = Ni_gas_tot(:,j).*k_Mw_gas(2:end); 
end
% Add water from evaporation (R_w is neagative)
Fi_gas      = [- R_w; Fi_gas];
R.gas       = Fi_gas;

%% CHAR COMBUSTION
% CONSUMPTION BY CHAR COMBUSTION 
% Kinetic parameters from "Kinetic study of biomass char combustion in a 
% low temperature fluidized bed reactor"
A           = 901067;       % [1/(s Pa^n)]
E_a         = 144.25E+06;   % [J/kmol]
n           = 0.59;         % [-]

% Air molar composition
w_a         = zeros(size(F_a));
for i = 1:length(T_w)
    w_a(:,i)= F_a(:,i)./sum(F_a(:,i));
end
y_a         = CALC_y_a(w_a,k_Mw_gas);
% Molar mass of air 
Mw_a        = zeros(size(T_w));
for i = 1:length(T_w)
    Mw_a(1,i)   = sum(y_a(:,i).*k_Mw_gas,1);
end
% Char moles
N_char      = M_char./k_Mw_w(1);
% Reaction rate (C + 0.5 O2 --> CO ) 
r           = A.*exp(-E_a./(R_ig.*T_w)).*(y_a(4,:).*k_p_amb).^n.*N_char; % [kmol/s]

% Oxygen available in primary air
for i = 1:length(T_w)
    if F_a(4,i)/k_Mw_gas(4) <= 0.5*r(i)
        r(i)= 2.*F_a(4,i)./k_Mw_gas(4);
    end
end

% Char consumtion rate in kg/s
R.char_cons = -r.*k_Mw_w(1);
% Account for gas produced/consumed by char burning
R.gas(4,:)  = R.gas(4,:) - 0.5.* r.*k_Mw_gas(4);
R.gas(7,:)  = R.gas(7,:) +  r.*k_Mw_gas(7);