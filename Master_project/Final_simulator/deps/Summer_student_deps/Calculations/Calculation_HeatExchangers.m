close all 
clc 

% ADD FOLDERS TO THE PATH
WorkDirectory=pwd;          
% Process data forlder
cd ..;cd Process_Data;DataDirectory=pwd;cd(WorkDirectory);
addpath(DataDirectory)   
% Function folder
cd ..;cd Functions;FunctionsDirectory=pwd;cd(WorkDirectory);
addpath(FunctionsDirectory)  

%% CALCULATE GRATE COOLING HEAT EXCHANGERS
% IMPORT DATA FROM EXCELL FILE
data    = importdata('Data_Boiler.txt'); 
data2   = importdata('Data_Water.txt'); 
data3   = importdata('Data_Fg.txt'); 
data4   = importdata('Data_Turbine.txt');
% Temperature upstream grate cooling
T_c     = Find_val('1HLC03CT001_',data);        % [C]
% Cooling water flow in the different zones
F_z1    = Find_val('1HLC51CF001_',data);        % [C]
F_z2    = Find_val('1HLC52CF001_',data);        % [C]
F_z3    = Find_val('1HLC53CF001_',data);        % [C]
% Cooling water temperature in the different zones
T_z1    = Find_val('1HLC51CT001_',data);        % [C]
T_z2    = Find_val('1HLC52CT001_',data);        % [C]
T_z3    = Find_val('1HLC53CT001_',data);        % [C]
        
% CALCULATIONS
% C_min in the different zones
C_z1    = F_z1.*4.2e3;
C_z2    = F_z2.*4.2e3;
C_z3    = F_z3.*4.2e3;
% Heat exchanged in the different zones
Q_z1    = C_z1.*(T_z1-T_c);
Q_z2    = C_z2.*(T_z2-T_c);
Q_z3    = C_z3.*(T_z3-T_c);
% Assumed effectiveness
epsilon = 0.03;
% Estimated grate temperature in the different zones
T_z1    = T_c + Q_z1./(epsilon.*C_z1)+273.15;   % [K]
T_z2    = T_c + Q_z2./(epsilon.*C_z2)+273.15;   % [K]
T_z3    = T_c + Q_z3./(epsilon.*C_z3)+273.15;   % [K]

% %% COMPARISON WITH EXPERIMENTAL DATA
% a       = load('F_fg_tot.mat');
% b       = load('T_fg.mat');
F_fg    = Find_val('1HTA11CF001_',data3)./(3.6*22.414).*28.966; % [kg/s]
% T_fg    = Find_val('1HBK10FT001_',data) + 273.15;               % [K]
% F_fg_c  = a.F_fg_tot.signals.values;
% T_fg_c  = b.T_fg.signals.values;
% t       = a.F_fg_tot.time;
% 
% figure(1);
% plot(t./3600,F_fg_c,'r');hold on 
% plot(300*[0:(length(F_fg)-1)]./3600,F_fg);
% xlabel('Time / h');
% ylabel('Flue gas mass flow / kg/s ');
% ylim([0 40]);
% legend('Computed','Measured');
% 
% figure(2);
% plot(t./3600,T_fg_c,'r');hold on 
% plot(300*[0:(length(F_fg)-1)]./3600,T_fg);
% xlabel('Time / h');
% ylabel('Flue gas temperature in combustion chamber / K ');
% legend('Computed','Measured');
            
%% CALCULATIONS EVAPORATOR 
[p_sat,T_sat,rho_vap,h_w,h_vap,c_p] = SatWater;
cp_fg   = 1.23e3;                                   % Specific heat capacity flue gas [J/kgK]
C_min   = F_fg.*cp_fg;       
T_fg   	= Find_val('1HBK10DT001_PV',data) + 273.15; % Temperature flue gas before heat recovery (calculated)
T_fg2   = (Find_val('1HBK40CT001_',data) + Find_val('1HBK40CT002_',data) + Find_val('1HBK40CT003_',data))./3 + 273.15; % Temperature flue gas downstream evaporator
Q       = C_min.*(T_fg-T_fg2);                      % Heat recovered in the evaporator
T_drum  = Find_val('1HAD10FT001_',data2)+ 273.15;  	% Temperature saturated water in the drum
epsilon_EVA     = Q./(C_min.*(T_fg-T_drum));        % Lumped effectiveness of the EVAPORATORS
epsilon_EVA_m   = mean(epsilon_EVA);

%% CALCULATIONS SUPERHEATER
T_fg3   = Find_val('1HBK40CT006_',data) + 273.15; 	% Temperature flue gas downstream superheater
F_w_SH  = Find_val('1LBA10CF001_',data2);           % Steam mass flow in the superheater [kg/s]
cp_steam= interp1(T_sat,c_p,T_drum,'linear','extrap');
T_w_SH  = Find_val('1LBA10CT901_',data2) + 273.15;  % Temperature superheated steam downstream superheater
Q1_SH   = F_w_SH.*cp_steam.*(T_w_SH - T_drum);      % 
Q2_SH   = C_min .* (T_fg2 - T_fg3);
epsilon_SH  = Q2_SH./(C_min.*(T_fg2-T_drum));
epsilon_SH_m= mean(epsilon_SH);

%% CALCULATIONS ECONOMIZER
T_fg4   = Find_val('1HNA10CT001_',data3) + 273.15; 	% Temperature flue gas downstream economizer
F_w_ECO = Find_val('1LAB40CF001_',data2);           % Water ass flow in the economizaer [kg/s]
cp_water= 4.2E+03;
T_w_ECO2= Find_val('1HAC20CT001_',data2) + 273.15;  % Temperature water downstream economizers
T_w_ECO1= Find_val('1LAB40CT001_',data2) + 273.15;  % Temperature water upstream economizers
Q1_ECO  = F_w_ECO.*cp_water.*(T_w_ECO2 - T_w_ECO1);
Q2_ECO  = C_min .* (T_fg3 - T_fg4);
epsilon_ECO = Q2_ECO./(C_min.*(T_fg3-T_w_ECO1));
epsilon_ECO_m   = mean(epsilon_ECO);

%% CALCULATIONS TURBINE
F_turb  = Find_val('1LBA10CF001_',data2);           % Steam mass flow in turbine
p1      = Find_val('1LBA20CP901XQ01',data4)*1e5+1e5;% Pressure before turbine
T1      = Find_val('1LBA20CT901XQ01',data4)+ 273.15;% Temprature before turbine
cp_vap  = interp1(p_sat,c_p,p1,'linear','extrap')
h_sat   = interp1(p_sat,h_vap,p1,'linear','extrap');
T_drum  = interp1(p_sat,T_sat,p1,'linear','extrap');
h_in    = h_sat + cp_vap.*(T_w_SH - T_drum);
p_HP    = Find_val('1MAA35CP901XQ01',data4)*1e5+1e5;% Pressure high pressure stage turbine
p_LP    = Find_val('1NDD10CP901XQ01',data4)*1e5+1e5;% Pressure low pressure stage turbine
h_HP    = interp1(p_sat,h_vap,p_HP,'linear','extrap');
h_LP    = interp1(p_sat,h_vap,p_LP,'linear','extrap');
Q_in    = F_turb.*(h_in - 0.5.*h_HP - 0.5.*h_LP);
Q_in2   = F_turb.*(h_in - h_HP );
W_el    = Find_val('1MKA10CE015XQ01',data4)*1e6;    % Electrict power turbine
eta_turb= W_el./Q_in;
eta_turb_m = mean(eta_turb); 
eta_turb2= W_el./Q_in2;
eta_turb2_m = mean(eta_turb2);
eta_turb_coef = polyfit(T_w_SH,eta_turb,1);
T2_b    = T1-W_el./(F_turb.*eta_turb_m);

%% CALCULATIONS LOW PRESSURE CONDENSER
T_LP    = Find_val('1NDD10CT001XQ01',data4)+ 273.15;% Temprature in LP condenser
T_LP2   = interp1(p_sat,T_sat,p_LP,'linear','extrap');

%% CALCULATIONS HIGH PRESSURE CONDENSER
p_DH    = Find_val('1NDA10CP001_',data4)*1e5;       % Water pressure in district heating
T_HP    = Find_val('1NDD20CT001XQ01',data4)+ 273.15;% Temprature in LP condenser
cp_water= 4.2E+03;
T_DH2   = Find_val('1NDA10CT001_',data4) + 273.15; 	% Temperature water to district heating
T_DH1   = Find_val('1NDB10CT003_',data4) + 273.15;  % Temperature water from district heating
F_w_DH  = Find_val('1NDA10CF010_A',data4);          % Water flow in district heating
Q_DH    = F_w_DH.*cp_water.*(T_DH2 - T_DH1); 
epsilon_DH  = Q_DH./(F_w_DH.*cp_water.*(T_HP-T_DH1));
epsilon_DH_m= mean(epsilon_DH);

% %% CALCULATIONS DISTRICT HEATING HEAT EXCHANGER 
% T_DH2   = Find_val('1NDA10CT001_',data4) + 273.15; 	% Temperature water to district heating
% F_w_DH  = Find_val('1NDA10CF010_A',data4);          % Water flow in district heating
% % T_w_turb= 
% cp_water= 4.2E+03;
% C_min   = F_w_DH.*cp_water;                         % Water on the boiler side condenses
% Q1_DH   = F_w_DH.*cp_water.*(T_DH2 - T_DH1);
% epsilon_DH  = Q1_DH/(F_w_DH.*cp_water.*(T_DH2-T_DH1));
% epsilon_DH_m= mean(epsilon_DH);
Q_DH2   = Find_val('1NDB10FQ011',data4);
% epsilon_DH  = Q1_DH./(C_min.*(T_w_turb-T_DH1));
figure(3);
plot(300*[0:(length(Q_DH)-1)]./3600,Q_DH2,'r');hold on 
plot(300*[0:(length(Q_DH)-1)]./3600,Q_DH./1e6);
xlabel('Time / h');
ylabel('Heat / MW ');
legend('Computed','Read');