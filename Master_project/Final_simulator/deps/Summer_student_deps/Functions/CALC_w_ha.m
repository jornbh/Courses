function w_ha =  CALC_w_ha(T,RH,Mw_gas)


% This function calculates the humid air mass compostion from the
% temperature and the desired relative humidity (RH)
% Dry air molar composition
x_dry   = [0; 
           0.79;
           0;
           0.21;
           0;
           0;
           0;
           0;
           0;
           0;
           0;
           0];

% Water molar fraction of saturated air at T
x_sat = CALC_xsat(T);

% Water molar fraction at T and RH
x_w   = RH.*x_sat./100;

% Humid air molar composition
x_ha  = [x_w; x_dry(2:end)*(1-x_w)];

% Humid air mass fraction
w_ha    = zeros(size(x_ha));
for i = 1:length(T)
    denom       = sum(x_ha(:,i).*Mw_gas,1);
    num         = x_ha(:,i).*Mw_gas;
    w_ha(:,i)   = num./denom;
end