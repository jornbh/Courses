clear all
run oppgave_1_d.m
close all
% size(G_eq)
% size(b_eq)
% size(A_eq)
% size(b_eq)
% size(A_eq)
%quadprog( G_eq, zeros(size(b_eq)), A_eq', b_eq, A_eq', b_eq ); 


OtherSolution = quadprog( G_eq, [],  [], [] , A_eq, b_eq); %Plottet blir nøyaktig det samme
    %Problmet er løsbart kun ved likheter, så quadprog trenger "ingen"
    %itterasjoner
    
