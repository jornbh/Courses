%% Script for making plots that give examples of how to analyse robustness when dealing with SISO transfer functions

%% Plant
P = ss(tf([1,-0.3], [1,1,1])*tf(1,[-1,1]));
tP = tf(P); 
Ps = (tf([1,-0.3], [1,1,1]));
%% Plot nyquist criteria
figure
ang = linspace(0,pi, 300);

ring_amp = max(abs(eig(P.A)))*1.3;
hold on 
plot(ring_amp*sin(ang), ring_amp*cos(ang), "k--")

plot([0,0], [-ring_amp, ring_amp], "k--")
plot( [-ring_amp, ring_amp], [0,0], "k-")

% Plot poles
labels =  split(sprintf("\\rho_%i,", 1:length(P.A)), ",");
labels = labels(1:end-1);
plot(real(eig(P.A)),imag(eig(P.A)), "ro"); 
text(real(eig(P.A)),imag(eig(P.A)),labels,'VerticalAlignment','bottom','HorizontalAlignment','right')


%Plot zeros
tf_zeros = (roots(tP.Numerator{1}));
zero_labels =  split(sprintf("\\lambda_%i,", 1:length(tf_zeros)), ",");
zero_labels = zero_labels(1:end-1);
plot(real(tf_zeros), imag(tf_zeros), "bo");
text(real(tf_zeros), imag(tf_zeros), zero_labels,'VerticalAlignment','bottom','HorizontalAlignment','right')

quiver([0,0],[-ring_amp,0], [0,0],[ring_amp,ring_amp], "k")
hold off
xlim([-ring_amp, ring_amp])
ylim([-ring_amp, ring_amp])


print("outputs/Fig_dump/Nyquist_criterion", "-depsc")

%% Make a generic bode plot 

figure 
bode(P)
print("outputs/Fig_dump/Generic_bode_plot", "-depsc")



%% Make generic N 
PID = pidtune(Ps, "PID");

[bode_amp, bode_phase, bode_angle] = bode(feedback(1,Ps*PID));
coplex_signal = squeeze(exp(1).^(bode_phase*1j) .* bode_amp); 
figure
hold on 
% plot(squeeze(bode_angle), abs(real(coplex_signal)), "b--")
% plot(squeeze(bode_angle), abs(imag(coplex_signal)), "r--")
plot(squeeze(bode_angle), abs(coplex_signal), "k")
hold off
set(gca, "XScale", "log")
set(gca, "YScale", "log")
title("|N(j\omega)|")

print("outputs/Fig_dump/Generic_N_plot", "-depsc")

%% Make generic M 
[bode_amp, bode_phase, bode_angle] = bode(feedback(Ps*PID,1));
coplex_signal = squeeze(exp(1).^(bode_phase*1j) .* bode_amp); 
figure
hold on 
% plot(squeeze(bode_angle), abs(real(coplex_signal)), "b--")
% plot(squeeze(bode_angle), abs(imag(coplex_signal)), "r--")
plot(squeeze(bode_angle), abs(coplex_signal), "k")
hold off
set(gca, "XScale", "log")
set(gca, "YScale", "log")
title("|M(j\omega)|")

print("outputs/Fig_dump/Generic_M_plot", "-depsc")

