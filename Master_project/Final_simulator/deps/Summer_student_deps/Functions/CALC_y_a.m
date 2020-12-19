function y_fg    = CALC_y_a(w_fg,Mw_gas)

y_fg    = zeros(size(w_fg));
for i = 1:size(w_fg,2)
    y_fg(:,i)= (w_fg(:,i)./Mw_gas)./(sum(w_fg(:,i)./Mw_gas,1));
end