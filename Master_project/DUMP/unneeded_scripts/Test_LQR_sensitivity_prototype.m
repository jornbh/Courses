% P = rss(10,2,3); 
K_lqry = lqry(P,1e8*eye(size(P.C,1)),eye(size(P.B,2)))

multipliers = linspace(0.3, 1.2, 50); 

[~,all_colours ] = meshgrid(1:length(P.A),multipliers);

%% Pole-changes with varying input-scaling
figure
hold on
for i = 2:length(multipliers)
    eigs = eig( P.A - P.B*K_lqry*multipliers(i));
    scatter(real(eigs), imag(eigs), 10, ones(size(eigs))*multipliers(i))
end
colorbar
set(gca, 'XScale', 'log')
colormap('jet');
title('Scaling sensitivity')
hold off

% %% Joking around with delays
% figure 
% hold on
% delays = linspace(1e-6, 200, 50)
% [a,b] = pade(1e-7,9)
% P0 = P*tf(a,b); 
% K_lqry_0 = lqry(P0,eye(size(P.C,1)),eye(size(P.B,2)))
% for i = 2:length(delays )
%     [a,b] = pade(delays(i)-delays(1),9)
%     Pd = P*tf(a,b); 
%     eigs = eig( Pd.A - Pd.B*K_lqry_0*delays(i)); 
%     [~, sorted_inds ] = sort(abs(eigs)); 
%     eigs = eigs(sorted_inds);
%     
%     eigs = eigs(1:length(P.A));
%     scatter(real(eigs), imag(eigs), 10, ones(size(eigs))*delays (i))
% %     scatter(real(all_poles(i,:)), imag(all_poles(i,:)), 50, all_colours(i,:) , 'filled');
%     
% end
% set(gca, 'XScale', 'log')
% colorbar
% colormap('jet');
% title('Sensitivity to delay')
% hold off