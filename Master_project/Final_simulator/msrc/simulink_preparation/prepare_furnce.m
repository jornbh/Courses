% clear all 
% close all
% clc

%%
global k 

% ADD SUB-FOLDERS TO THE PATH
addpath(genpath(pwd));
load_system("Furnace")
%%


disp("Computing ss_state"); 
set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch', "sw","0")    
set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch1', "sw","0")    
ss_state = create_ss_state(19e3);
set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch', "sw","1")    
set_param('Furnace/Controller/MPC_subsystem/LQG_controller/Manual Switch1', "sw","1")    

%%

% LOAD USEFUL PARAMETERS
FurnaceParameters;

%% My new stuff 
% Use initial state and set the parameters properly 
set_param("Furnace", "LoadInitialState", "on", ...
    "StopTime", "27000"); % Removes some of the manual work

%%
"Summer project initialized"
% %%
% "Start sim"
% tic
% sim_results = sim("Furnace"); 
% toc
% "End sim"

% Do not forget balred(sys,order) to reduce the order of a system

