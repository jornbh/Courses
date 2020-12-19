close all
clc
clear all

% ADD FOLDERS TO THE PATH
WorkDirectory=pwd;
% Process data forlder
cd ..;cd Process_Data;DataDirectory=pwd;cd(WorkDirectory);
addpath(DataDirectory)
% Function folder
cd ..;cd Functions;FunctionsDirectory=pwd;cd(WorkDirectory);
addpath(FunctionsDirectory)

FurnaceParameters;
%% IMPORT DATA
data    = importdata('Data_Fg.txt');
data2   = importdata('Data_Boiler.txt');
data3   = importdata('Data_Water.txt');
data4   = importdata('Data_Turbine.txt');

%% MONITORING STATION - Measured O2 and CO2 are considered to be dry
% Dry gas composition 
Y_O2_2_dry  = Find_val('1HNE50ACT_U08',data)./100;
Y_H2O_2  = Find_val('1HNE50CQ403_',data)./100; 
k_O2     = (0.21-Y_O2_2_dry)./(0.21-0.11);
k_H2O    = 1./(1-Y_H2O_2);
Y_CO2_2_dry = Find_val('1HNE50CQ409_',data)./100;          % [vol%]
Y_N2_2_dry  = 1 - Y_CO2_2_dry - Y_O2_2_dry ;
Y_fg2_dry= [zeros(length(Y_N2_2_dry),1) Y_N2_2_dry Y_CO2_2_dry Y_O2_2_dry zeros(length(Y_N2_2_dry),(length(k.Mw_gas)-4))];
% Wet gas composition
Y_fg2    = [Y_H2O_2 (1-Y_H2O_2).*Y_fg2_dry(:,2:end)];
Mw_fg2   = (Y_fg2 *k.Mw_gas);
Y_N2_2   = Y_fg2(:,2);
Y_O2_2   = Y_fg2(:,4);
Y_CO2_2  = Y_fg2(:,3);
F_fg2_dry= Find_val('1HNE50CF401_A1',data).*1e3;        % [Nm3/h]
F_fg2    = F_fg2_dry./(1-Y_H2O_2);
Mw_fg2_dry= (Y_fg2_dry *k.Mw_gas);
% 
% %% MONITORING STATION - Measured O2 and CO2 are considered to be wet
% % Wet gas composition
% Y_H2O_2  = Find_val('1HNE50CQ403_',data)./100; 
% Y_CO2_2  = Find_val('1HNE50CQ409_',data)./100;          % [vol%]
% Y_O2_2   = Find_val('1HNE50ACT_U08',data)./100;
% Y_N2_2   = 1 - Y_CO2_2 - Y_O2_2 - Y_H2O_2;
% Y_fg2    = [Y_H2O_2 Y_N2_2 Y_CO2_2 Y_O2_2 zeros(length(Y_N2_2),(length(k.Mw_gas)-4))];
% Mw_fg2   = (Y_fg2 *k.Mw_gas);
% % Dry gas composition 
% Y_fg2_dry= [zeros(length(Y_N2_2),1) Y_fg2(:,2:end)./(1-Y_H2O_2) ];
% Y_O2_2_dry= Y_fg2_dry(:,4);
% Y_N2_2_dry= Y_fg2_dry(:,2);
% F_fg2_dry= Find_val('1HNE50CF401_A1',data).*1e3;        % [Nm3/h]
% F_fg2    = F_fg2_dry./(1-Y_H2O_2);

%% AFTER BOILER
% Dry gas composition
Y_O2_dry= Find_val('1HNA10FQ001_',data2)./100;          % [vol%] 
R       = (0.21-Y_O2_2_dry)./(Y_O2_2_dry-Y_O2_dry);
T_fg    = Find_val('1HNA10FT001_',data)+ 273.15;     	% [K] 
Y_H2O   = Find_val('1HNA10CQ003_',data)./100;           % [vol%]
Y_N2_dry= (1+1./R).*Y_N2_2_dry-0.79./R;
Y_CO2_dry = 1 - Y_O2_dry - Y_N2_dry;
Y_fg_dry= [zeros(length(Y_N2_2),1) Y_N2_dry Y_CO2_dry Y_O2_dry zeros(length(Y_N2_2),(length(k.Mw_gas)-4))];
% Wet gas composition
Y_O2    = Find_val('1HNA10CQ005_AI',data2)./100;        % [vol%] % This is not used in calculations
Y_H2O_calculated = (1-Y_O2./Y_O2_dry);
err_H2O = (Y_H2O_calculated-Y_H2O)./Y_H2O.*100;         % Mean error: 1.6%, max err: 12%
F_fg    = Find_val('1HTA11CF001_',data).*1e3;           % [Nm3/h]
Y_fg    = [Y_H2O (1-Y_H2O).*Y_fg_dry(:,2:end)];
Y_N2    = Y_fg(:,2);
Y_CO2   = Y_fg(:,3);
Mw_fg   = (Y_fg *k.Mw_gas);

%% AIR
F_air   = Find_val('1HLA01FF001_F_',data2);     % [Nm3/h]
T_air   = Find_val('1HLA10CT001_F_',data2)+ 273.15;
N_air   = F_air./(22.414.*3600);                % [kmol/s]
w_air   = CALC_w_ha(300,50,k.Mw_gas);           % T = 300K, RH = 90%
Y_air   = CALC_y_a(w_air,k.Mw_gas)';            % [kmol/kmol_air]
Y_H2O_air= Y_air(1);                          
Y_N2_air= Y_air(2);
Y_O2_air= Y_air(4);
Mw_a    = sum(Y_air*k.Mw_gas,2);                % [kg/kmol_air]
M_air   = F_air./(3600*22.414).*Mw_a;

%% VAN KESSEL METHOD
eta_C 	= 1;
y     	= 2.1.*ones(size(Y_N2));
F_fg_calc = Y_N2_air./(Y_N2).*(F_air);          % [Nm3/h]
M_fg_calc = F_fg_calc./(3600*22.414).*Mw_fg;
N_fg  	= F_fg_calc./(22.414.*3600);          % [kmol/s]
N_CHyOz = N_fg.*Y_CO2./eta_C;                 % [kmol/s] of combustible waste
z2     	= 2.*eta_C+0.5.*y-2.*eta_C.*(Y_N2.*Y_O2_air./Y_N2_air-Y_O2)./Y_CO2;
z       = 2.*eta_C+0.5.*y-2.*(N_air.*Y_O2_air-N_fg.*Y_O2)./N_CHyOz;
for i = 1:length(z)
    if z(i) <=0
        z(i)    = 0.01;
        y(i)    = 2.*z(i)-4.*eta_C+4.*(N_air(i).*Y_O2_air-N_fg(i).*Y_O2(i))./N_CHyOz(i);
    end
end
Mw_CHyOz= k.Mw_w(1) + y.*k.Mw_w(2) + z.*k.Mw_w(6);          % [kg/kmol]
F_CHyOz = N_CHyOz.*Mw_CHyOz;                                % [kg/s]
H_CHyOz = (408.4 + 102.4.*y - 156.8.*z).*1e6./Mw_CHyOz;     % [J/kg]
N_moist = N_fg.*Y_H2O - N_air.*Y_H2O_air - 0.5.*y.*N_CHyOz; % [kmol/s]
w_moist_ashfree = (N_moist.*k.Mw_water)./(N_moist.*k.Mw_water + N_CHyOz.*Mw_CHyOz);
w_comb_ashfree  = (1-w_moist_ashfree).*[k.Mw_w(1)./Mw_CHyOz y.*k.Mw_w(2)./Mw_CHyOz z.*k.Mw_w(6)./Mw_CHyOz];
H_ashfree       = (1-w_moist_ashfree).*H_CHyOz + w_moist_ashfree.*k.DeltaH(1);
k.w_moist_ashfree = w_moist_ashfree;
k.w_comb_ashfree  = w_comb_ashfree;
F_ashfree = F_CHyOz./(1- w_moist_ashfree);
F_waste = F_ashfree./0.78;                                  % 22% ashes
F_w     = Find_val('1HFB10FF002',data2)./3.6;               % [kg/s]

%% LOAD SIMULATION DATA
a   	= load('F_fg_out');
F_fg_out= sum(a.F_fg_out.signals.values,2);
x       = a.F_fg_out.time/3600;
xx      = linspace(x(1),x(end),length(F_air));

%% Flue gas 
figure(1)
plot(xx,M_air,'Linewidth',1);hold on
% plot(F_air./(3600*22.414).*28.966+F_waste./3.6.*0.7)
plot(xx,F_fg2./(3600*22.414).*Mw_fg2,'Linewidth',1);
plot(xx,F_fg./(3600*22.414).*Mw_fg,'Linewidth',1);
plot(xx,M_fg_calc,'Linewidth',1);
plot(x,F_fg_out,'Linewidth',1);
ylim([25,45]);
xlabel('Time / h');
ylabel('Mass flows / kg s^{-1}');
xlim([0 24]);
legend('DATA - input air','DATA - flue gas at stack','DATA - flue gas after boiler','CALC - Van Kessel','CALC - Dynamic model','Location','southeast')

%% Heat production
Q_waste   = Find_val('1HFB10FQ004',data2);
Q_fuel    = F_CHyOz.*H_CHyOz;
Q_recovered = Q_fuel + M_air.*k.cp_fg.*(T_air-k.T_amb) - M_fg_calc.*k.cp_fg.*(T_fg-k.T_amb);
F_steam   = Find_val('1LBA10CF001_',data3);             % [kg/s]
T_steam   = Find_val('1LBA10CT901_',data3) + 273.15;    % [K]
F_water   = Find_val('1LAB40CF001_',data3);             % [kg/s]
T_water   = Find_val('1LAB40CT001_',data3) + 273.15;    % [K]
T_drum    = Find_val('1HAD10FT001_',data3) + 273.15;    % [K]
[~,T_sat,~,~,~,c_p] = SatWater;
cp_vap    = interp1(T_sat,c_p,T_drum,'linear','extrap');
Q_rec_water = F_water.*(k.cp_water.*(T_drum-T_water)+1.639*1e6 ) + F_steam.*(cp_vap.*(T_steam-T_drum));
Q_rec_water2= F_steam.*(k.cp_water.*(T_drum-T_water) ) + F_steam.*(cp_vap.*(T_steam-T_drum)- k.DeltaH(1));
W_turb    = Find_val('1MKA10CE015XQ01',data4).*1e6;     % [W]
T_DH1     = Find_val('1NDB10CT003_',data4) + 273.15;    % [K]
T_DH2     = Find_val('1NDA10CT001_',data4) + 273.15;    % [K]
F_DH      = Find_val('1NDA10CF010_A',data4);            % [kg/s]
Q_DH      = F_DH.*k.cp_water.*(T_DH2-T_DH1);
figure(2)
plot(xx,Q_waste,'Linewidth',1);hold on
plot(xx,Q_fuel.*1e-6,'Linewidth',1);
plot(xx,Q_recovered.*1e-6,'Linewidth',1);
plot(xx,Q_rec_water.*1e-6,'Linewidth',1);
plot(xx,(Q_DH+W_turb).*1e-6,'Linewidth',1);
plot(xx,Q_rec_water2.*1e-6,'Linewidth',1);
xlabel('Time / h');
ylabel('Heat / MW');
legend('DATA - Gross heat capacity waste','CALC - Waste HHV (Van Kessel)','CALC - Recovered heat - gas side','DATA - Recovered heat - water side','DATA - Delivered heat + electricity','Location','northeast')
xlabel('Time / h');
ylabel('Waste / kgs^{-1}');

%% Waste flow
figure(3)
plot(xx,F_waste,'Linewidth',1);hold on
plot(xx,F_w,'Linewidth',1);
legend('CALC - Waste flow','DATA - Waste flow');

figure(4)
plot(xx,F_steam,'Linewidth',1);
xlabel('Time / h');
xlim([0 24]);
ylabel('Steam production / kgs^{-1}');

figure(5)
plot(xx(1:13).*60,F_steam(1:13),'Linewidth',1);
xlabel('Time / min');
ylabel('Steam production / kgs^{-1}');
% % FLUE GAS MOLAR FLOW (as calculated according to Van Kessel)

% N_air   = F_air./(22.414.*3.6);                 % [kmol/s]
% x_sat   = CALC_xsat(273.15+10);
% Y_H2O_air= x_sat.*0.9;                          % 90% Relative Humidity
% Y_N2_air= 0.79.*(1-Y_H2O_air);
% Y_O2_air= 1 - Y_H2O_air - Y_N2_air;
% Y_air = [Y_H2O_air;       % H2O
%          Y_N2_air;        % N2
%          0;               % CO2
%          Y_O2_air;        % O2
%          zeros(8,1)];     % others
% Y_fg  = [Y_H2O Y_N2 Y_CO2 Y_O2 zeros(length(Y_N2),8)];     % others
% F_fg3   = Y_N2_air./Y_N2.*(F_air);              % [kNm3/h]
% N_fg    = F_fg3./(22.414.*3.6);                 % [kmol/s]
% 
% % COMBUSTABLE FRACTION MOLAR FLOW
% eta_c   = 0.95;
% N_CHyOz = Y_CO2./eta_c.*Y_N2_air./Y_N2.*N_air;  % [kmol_CHyOz/s]
% 
% % COMBUSTABLE FRACTION COMPOSITION
% y       = 1.78;
% z       = 0.5.*y + 2.*eta_c./(1-Y_O2_air./(1-Y_H2O_air)) .* (1-(Y_O2_air./(1-Y_H2O_air)-Y_O2./(1-Y_H2O))./(Y_CO2./(1-Y_H2O)));
% 
% 
% 
% F_air2  = Find_val('1HLA10CF001_F_',data2)+Find_val('1HLA82CF001_F_',data2)+Find_val('1HLA81CF001_F_',data2);
% Y_N2b   = 0.7815.*F_air./(F_fg.*1e3);
% 
% F_waste = Find_val('1HFB10FF002',data2);        % [tonn/h]
% 
% % CO/CO2 calculations
% % Measurements at monitoring station
% F_CO2  = Find_val('1HNE50CQ409_',data);         % [mg/Nm3]
% F_CO   = Find_val('1HNE50CQ405A_',data);        % [mg/Nm3]
% F_CO_corr  = Find_val('1HNE50ACT_U01',data);    % [mg/Nm3] corrected for 11% O2
% % Measurements after boiler (they are basically the same measurement)
% F_CO_b  = Find_val('1HNA10CQ006_',data2);         % [mg/m3]
% F_CO_b2  = Find_val('1HNA10CQ002_AI',data2);      % [mg/m3]
% T_fg_b   = Find_val('1HNA10CT001_',data) + 273.15; % [K]
% F_CO_bN  = F_CO_b.*T_fg_b./293.15;
% 
a=load('F_fg_out');
F_fg_out = sum(a.F_fg_out.signals.values,2);
x=linspace(0,length(Y_CO2_2),length(F_fg_out));
% figure(1)
% plot(F_CO_b,'r'); hold on
% plot(F_CO,'k');
% legend('boiler','monitoring')
% 
% Mw_a    = sum (Y_air.*k.Mw_gas);
% Mw_fg   =  (Y_fg *k.Mw_gas);
% % Flue gas measurements
% % After boiler
% F_fg    = Find_val('1HTA11CF001_',data).*1e3;        % [kNm3/h]
% % At monitoring station
% F_fg2   = Find_val('1HNE50CF401_A1',data).*1e3;      % [kNm3/h]
% figure(2)
% plot(F_air,'Linewidth',1);hold on
% plot(F_fg2,'Linewidth',1);
% plot(F_fg,'Linewidth',1);hold on 
% plot(F_fg3,'Linewidth',1);
% plot(F_fg_react)
% legend('input air','flue gas at stack','flue gas after boiler', 'calculated')
% figure(3)
% plot(F_air./(3600*22.414).*28.966,'Linewidth',1);hold on
% % plot(F_air./(3600*22.414).*28.966+F_waste./3.6.*0.7)
% plot(F_fg2./(3600*22.414).*28.966,'Linewidth',1);
% plot(F_fg./(3600*22.414).*28.966,'Linewidth',1);
% % plot(F_fg3./(3600*22.414).*28.966,'Linewidth',1);
% plot(x,F_fg_out,'Linewidth',1);
% ylim([28,39]);
% % % legend('input air','air+70%waste','flue gas at stack','flue gas after boiler', 'van Kessek method','calculated')
% legend('input air','flue gas at stack','flue gas after boiler','calculated')
% 
% Y_fg_dry  = [zeros(length(Y_N2),1) Y_fg(:,2:end)]./sum(Y_fg(:,2:end),2);
% Mw_fg_dry  = (Y_fg_dry *k.Mw_gas);
% Mw_a_dry = 0.79.*k.Mw_gas(2)+0.21.*k.Mw_gas(4);
% figure(4)
% plot(F_air.*(1-Y_H2O_air)./(3600*22.414).*Mw_a_dry,'Linewidth',1);hold on
% plot(F_fg2.*(1-Y_H2O_2)./(3600*22.414).*Mw_fg2_dry,'Linewidth',1);hold on
% plot(F_fg.*(1-Y_H2O)./(3600*22.414).*Mw_fg_dry,'Linewidth',1); 
% legend('input air','flue gas at stack','flue gas after boiler')
% % figure(1)
% % plot(F_CO2./max(F_CO2),'r'); hold on
% % plot(F_waste./max(F_waste),'k');
% %
% % figure(2)
% % plot(F_N2./max(F_N2),'r'); hold on
% % % plot(F_O2./max(F_O2),'k');
% % plot(F_fg./max(F_fg),'b');
% % % plot(F_CO2./max(F_CO2),'b');
% % % plot(F_H2O./max(F_H2O),'g');
% %
% % figure(3);hold on
% % plot(Y_N2,'g');
% % plot(Y_N2b,'r');
% % % plot(Y_N2c,'k');
% %
% figure(4);hold on
% plot(F_fg.*1e3,'g');
% plot(F_fg3,'c');
% plot(F_air,'r');
% ylabel('kNm3/h');
% % plot(F_air2,'b');
% legend('after boiler-process data','calculated','total air');
% 
% figure(5);hold on
% plot(F_fg.*1e3,'g');
% plot(F_fg2.*1e3,'r');
% plot(F_fg3,'b');
% legend('after boiler','after rensing','calculated');
% %
% % figure(6);hold on
% % plot(F_O2);
% % plot(F_H2O,'b');
% % plot(F_CO2,'g');
% % plot(F_N2,'k');
% %
% % figure(7);hold on
% % plot(O2_vol);
% % plot(H2O_vol);