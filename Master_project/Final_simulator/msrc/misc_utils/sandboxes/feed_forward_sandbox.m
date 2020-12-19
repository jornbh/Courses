%% See "proper_feed_forward.m" for the real implementation

%% Plant dynamics 
P = tf(rss(10)); 
G = tf(rss(10)); 

%%

% step(P)

P_zeros = roots(P.numerator{1})
P_poles = roots(P.denominator{1})

is_good_pole = real(P_zeros) <0
good_inv_P_unscaled = tf( poly(P_poles), poly(P_zeros(is_good_pole))); % The gain may be wrong

P_gain = P.numerator{1}(end)/ P.Denominator{1}(end); 
bad_P_zeros = P_zeros(~is_good_pole); 
bad_cancelation=  tf( 1, poly(-bad_P_zeros*1e0)); % An attempt to remove the issue (?) 
inv_P_unscaled = good_inv_P_unscaled * bad_cancelation; 


if ~isproper(inv_P_unscaled)
    relative_degree = sum(cumprod( (inv_P_unscaled.Denominator{1} == 0))); 
    [a,b] = butter(relative_degree,1e7,"s"); 
    properness_filter = tf(a,b); 
    inv_P_unscaled = inv_P_unscaled*properness_filter; 
    
end

inv_P_gain = inv_P_unscaled.numerator{1}(end)/ inv_P_unscaled.Denominator{1}(end); 
inv_P = inv_P_unscaled * 1/(P_gain*inv_P_gain)


figure 
hold on 
step(G)
step(G - P*inv_P*G)
legend("Undamped", "Damped")