function R = CALC_Rw_Gas(M_fg,T_fg,nc_gas,Mw_gas,V_FreeBoard)

R_gas= 8.314462175*1E3;
for i =1:nc_gas
    if M_fg(i) < 0
        M_fg(i) =0;
    end
end
% Molar density [kmol/m3]
c_fg    = M_fg./(Mw_gas.*V_FreeBoard);

% Molar density in [mol/m3]
c_fg    = c_fg.*1e3;

%% Arrhenius parameters
% Reaction parameters from "Jones and Lindstedt"
A       = zeros(4,1);
E_a     = zeros(4,1);
b       = zeros(4,1);
% H2  + 1/2O2 --> H2O
A(1)    = 0.25E17;
E_a(1)  = 40E6.*4.184;
b(1)    = -1;
% CO  + H2O   --> CO2 + H2
A(2)    = 0.275E10;
E_a(2)  = 20E6.*4.184;
b(2)    = 0;
% CH4 + H2O   --> CO  + 3H2
A(3)    = 0.30E9;
E_a(3)  = 30E6.*4.184;
b(3)    = 0;
% CH4 + 1/2O2 --> CO  + 2H2
A(4)    = 0.44E12;
E_a(4)  = 30E6.*4.184;
b(4)    = 0;

% Reaction constants [mol^n/s]
k_f     = A.*T_fg.^b.*exp(-E_a./(R_gas.*T_fg));


% Reaction rates [mol/s]
R_mol   = [k_f(1).*c_fg(6).^(1/2).*c_fg(4).^(9/4).*c_fg(1).^(-1);   % H2  + 1/2O2 --> H2O                  
           k_f(2).*c_fg(7).*c_fg(1);                                % CO  + H2O   --> CO2 + H2
           k_f(3).*c_fg(5).*c_fg(1);                                % CH4 + H2O   --> CO  + 3H2
           k_f(4).*c_fg(5).^(1/2).*c_fg(4).^(5/4)];                 % CH4 + 1/2O2 --> CO  + 2H2

% Component molar reaction fluxes [mol/s]
rr_mol  = [+R_mol(1) - R_mol(2) - R_mol(3);             % H2O
           0;                                           % N2
           R_mol(2);                                    % CO2
           - 0.5.*R_mol(1) - 0.5.*R_mol(4);             % O2
           - R_mol(3) - R_mol(4);                       % CH4
           -R_mol(1)+R_mol(2)+3.*R_mol(3)+2.*R_mol(4);  % H2
           - R_mol(2) + R_mol(3) + R_mol(4);            % CO
           0;                                           % H2S
           0;                                           % HCl
           0;                                           % HF
           0;                                           % NOx
           0];                                          % NH3
% Component molar reaction fluxes in [kmol/s]
rr_mol  = rr_mol.*1e-3;
       
% Component mass reaction fluxes [kg/s]
R.R     = rr_mol.*Mw_gas;   

%% Enthalpy of reaction [J/mol]
H_reac  = [+242000;
           + 41150;
           -206830;
           + 35170];

Delta_H = R_mol.*H_reac;
R.H     = sum(Delta_H);