clear all
close all
clc

n_cell  = 15;
n_comp  = 9;
M_w     = load('M_waste');
M_water = [];
for i=1:n_cell
    M_water = [M_water M_w.M_waste.signals.values(:,(i-1).*n_comp+1)];
end
M_water_int= [];
x1 = 0:1:14;
x2 = linspace(0,15,30);
% for i = 1:length(M_water(:,1))
%     M_water_int = [M_water_int; interp1(x1,M_water(i,:),x2,'linear','extrap')];
% end
M_water_int = interp1(x1,M_water(end,:),x2,'linear','extrap');
figure(1); hold on
plot(x2,M_water_int(end,:))
