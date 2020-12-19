% There is not enough test data, so some more has to be generated, but it seems unreasonable if we make it into pure white noise, so let's copy the frequency characteristics instead

% tsin is the timeseries we base oursleves on
transformed =  fft(tsin.data- mean(tsin.data));% Ignore the mean for now
dt = mean(diff(tsin.time)); % Sampling time
freqs = (2*pi/dt) * tsin.time./tsin.time(end); % The real frequencies the data represents



% We want to make the data into a filter
Ftsin = timeseries(abs(transformed), freqs); % Make the frequencies into a timeseries 
% % mean(transformed) 
% hold on 
% plot(idealfilter(Ftsin,  [0,1e2], "pass") + mean(Ftsin), "bx-")
% plot(            Ftsin, "r--")

% hold off

% Since the input is some kind of noise, the frequency amplitudes will be a bit random

% So we filter the fourier transform 
simplified_noise_characteristics = mean(Ftsin.data) + idealfilter(Ftsin,  [0,1e2], "pass"); 

% Now we need some random signal, that we can give the characteristics of the rest of the data
rT_end = tsin.time(end)*2; 
for rdt = [30,300,900, 30]; 
rtime = 0:rdt:rT_end; 
r = rand(size(rtime'));
Fr_unscaled = fft(r - mean(r));
r_power = sqrt(mean(abs(Fr_unscaled).^2)); 
Fr = Fr_unscaled ./r_power; % Make sure the random signals have the proper amplitude
rfreqs = (2*pi/rdt) * rtime./rtime(end);  
su = sum(rfreqs <= freqs(end))
si = length(rfreqs)

end
% Next, we have fit the characteristics into a new set of sampling-times and time-horizons

resample_freqs = linspace( 0, freqs(end), sum(rfreqs <= freqs(end))); 
resampled_filter = resample( simplified_noise_characteristics, resample_freqs); 
% set(gca, "YScale", "log")

% close all 
% hold off
% hold on 
% plot(resampled_filter, "ro")
% plot(simplified_noise_characteristics, "bx-")
% legend("Resampled", "Original")
