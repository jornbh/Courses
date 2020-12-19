function x_sat = CALC_xsat(T)

% This function calculates the water molar fraction of saturated air at ambient
% pressure as a function of temperature, by interpolating experimental data

T_ref = [-40, -30:5:90, 100]  + 273.15;
x_ref = [12.84 38 63.25 103.2 165.2 259.2 401.5 610.8 871.9 1227 1704 2337 3167 4243 5623 7378 9585 12339 14745 19925 25014 31167 38554 47365 57809 70112 101325]./101325;

x_sat = interp1(T_ref,x_ref,T,'pchip');