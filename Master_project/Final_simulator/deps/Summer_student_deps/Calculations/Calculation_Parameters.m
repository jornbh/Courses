close all
clear all
data    = importdata('Data_hourly.txt');

% Economizer
F_fg    = Find_val('1HTA11CF001_',data)./(3.6*22.414).*28.966; % [kg/s]
cp_fg   = 1.23e3;                                   % Specific heat capacity flue gas [J/kgK]
C_min   = F_fg.*cp_fg;
T_fg3   = Find_val('1HBK40CT006_',data) + 273.15; 	% Temperature flue gas downstream superheater
T_fg4   = Find_val('1HNA10CT001_',data) + 273.15; 	% Temperature flue gas downstream economizer
F_w_ECO = Find_val('1LAB40CF001_',data);           % Water ass flow in the economizaer [kg/s]
cp_water= 4.2E+03;
T_w_ECO2= Find_val('1HAC20CT001_',data) + 273.15;  % Temperature water downstream economizers
T_w_ECO1= Find_val('1LAB40CT001_',data) + 273.15;  % Temperature water upstream economizers
Q1_ECO  = F_w_ECO.*cp_water.*(T_w_ECO2 - T_w_ECO1);
Q2_ECO  = C_min .* (T_fg3 - T_fg4);
epsilon_ECO = Q2_ECO./(C_min.*(T_fg3-T_w_ECO1));
epsilon_ECO2= Q1_ECO./(C_min.*(T_fg3-T_w_ECO1));
epsilon_ECO = [epsilon_ECO(1:3500);epsilon_ECO(5000:end)];
epsilon_ECO2= [epsilon_ECO2(1:3500);epsilon_ECO2(5000:end)];
epsilon_ECO_m   = mean(epsilon_ECO);
eff_ECO     = Q1_ECO./Q2_ECO;
eff_ECO_m   = mean(eff_ECO(1:3500));
eff_ECO_m2  = mean(eff_ECO(5000:end));
eff_ECO     = [eff_ECO(1:3500);eff_ECO(5000:end)];
C = [C_min(1:3500);C_min(5000:end)].*[T_fg3(1:3500);T_fg3(5000:end)];
% figure(1)
% plot((Q1_ECO),'*');hold on;
% plot(Q2_ECO,'x')
% lsline
% ylim([4e6 8e6])
% legend('cold side', 'hot side');
% h = line;
% s = h.LineStyle;
% h.LineStyle = ':';
% figure(2)
% plot(eff_ECO,'*');hold on;
% plot(epsilon_ECO,'x');hold on;
% plot(epsilon_ECO2,'o');hold on;
% lsline
% ylim([0 1]);
figure(1)
plot(F_fg)
% figure(3);
% plot([F_fg(1:3500);F_fg(5000:end)],eff_ECO);
% 
% figure(4);hold on
% plot(T_w_ECO1)
% plot(T_fg3);
% T_w_ECO = [T_w_ECO1(1:3000);T_w_ECO1(5000:end)];
% Tmin = min(T_w_ECO)
% Tmax = max(T_w_ECO)
% 
% T_drum  = Find_val('1HAD10FT001_',data)+ 273.15;
% 
% figure(5);
% plot(T_drum)
% ylim([540 550]);
% 
% T_w_SH  = Find_val('1LBA10CT901_',data) + 273.15;
% [p_sat,T_sat,rho_vap,h_w,h_vap,c_p] = SatWater;
% F_turb  = Find_val('1LBA10CF001_',data);           % Steam mass flow in turbine
% p1      = Find_val('1LBA20CP901XQ01',data)*1e5+1e5;% Pressure before turbine
% T1      = Find_val('1LBA20CT901XQ01',data)+ 273.15;% Temprature before turbine
% cp_vap  = interp1(p_sat,c_p,p1,'linear','extrap');
% h_sat   = interp1(p_sat,h_vap,p1,'linear','extrap');
% T_drum  = interp1(p_sat,T_sat,p1,'linear','extrap');
% h_in    = h_sat + cp_vap.*(T_w_SH - T_drum);
% p_HP    = Find_val('1MAA35CP901XQ01',data)*1e5+1e5;% Pressure high pressure stage turbine
% p_LP    = Find_val('1NDD10CP901XQ01',data)*1e5+1e5;% Pressure low pressure stage turbine
% h_HP    = interp1(p_sat,h_vap,p_HP,'linear','extrap');
% h_LP    = interp1(p_sat,h_vap,p_LP,'linear','extrap');
% Q_in    = F_turb.*(h_in - 0.5.*h_HP - 0.5.*h_LP);
% W_el    = Find_val('1MKA10CE015XQ01',data)*1e6;    % Electrict power turbine
% eta_turb= W_el./Q_in;
% eta_turb_sel  = eta_turb(eta_turb(:,1)>= 0.8,:);
% p_HP     = p_HP(eta_turb(:,1)>= 0.8,:);
% p_LP     = p_LP(eta_turb(:,1)>= 0.8,:);
% eta_turb_sel2  = eta_turb_sel(eta_turb_sel(:,1)<= 1,:);
% p_HP     = p_HP(eta_turb_sel(:,1)<= 1,:);
% p_LP     = p_LP(eta_turb_sel(:,1)<= 1,:);
% eta_turb_m = mean(eta_turb_sel2); 
% B = regress(eta_turb_sel2,[p_HP,p_LP]);
% eta_comp = [p_HP,p_LP]*B;
% figure(6);
% plot(p1)
% figure(7);
% plot(p_LP,eta_turb_sel2,'o');hold on 
% plot(p_HP,eta_turb_sel2,'o')
% lsline
% figure(8);hold on
% plot(eta_turb_sel2,'x')
% plot(eta_comp,'-k')