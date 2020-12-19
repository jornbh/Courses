function k = CALC_DeltaH(k)


% Heat of pyrolysis from Per's file "Na2CO3_Package"
% --> need something better here
DeltaH_pyro = 5.41E+06;  % [J/kg]

% Heat of evaporation from "Bryden, K.M., Ragland, K.W. and Rutland, C.J., 
% 2002. Modeling thermally thick pyrolysis of wood. Biomass and Bioenergy,
% 22(1), pp.41-53."
DeltaH_H2O  = -2.44E+06;    % J/kg

% Heat of char combustion calculated from enthalpies of formations
DeltaH_Char = 110.63E+6;    % J/kmol
DeltaH_Char = DeltaH_Char./k.Mw_w(1); % J/kg

k.DeltaH      = [DeltaH_H2O;
               DeltaH_pyro;
               DeltaH_Char];