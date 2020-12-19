%% Makes feed forward controller if the variable "full_estimated_plant" is available
full_estimated_plant;
AB_scale = 1; 
steam_G_B = zeros([7,3]);
steam_G_B(5:end,:) = eye(3); 
steam_G_list = [1,0,0]* full_estimated_plant* steam_G_B;
steam_P = [1,0,0]*full_estimated_plant * [-1,0,AB_scale,0, zeros([1,3])]'; 

Q_1_ff = make_feed_forward(steam_P, steam_G_list*[1;0;0]); 
Q_2_ff = make_feed_forward(steam_P, steam_G_list*[0;1;0]); 
Q_3_ff = make_feed_forward(steam_P, steam_G_list*[0;0;1]); 

O2_P = [0,1,0] *full_estimated_plant* [1;0;1;0; zeros([3,1])];
O2_G = [0,1,0] *full_estimated_plant* [-1,0,AB_scale,0, zeros([1,3])]'; 

O2_ff = make_feed_forward(O2_P, O2_G); 

%% Handle the FF 
steam_ff_battery = [1,1,1]* blkdiag(Q_1_ff,Q_2_ff,Q_3_ff);
full_feed_forward_controller = [1; - O2_ff] * steam_ff_battery; 
MIMO_P = full_estimated_plant *  [
                            -1,0,AB_scale,0, zeros([1,3]);
                            1,0,1,zeros([1,4])]'; 
MIMO_G = full_estimated_plant* steam_G_B;

figure 
step(MIMO_G); 
hold on 

step( MIMO_G -  MIMO_P *full_feed_forward_controller )
 legend("Undamped", "Damped")
hold off 
%% Nice controller names 
Q_grate_to_delta_F_a_feed_forward_controller = [-1; AB_scale]*steam_ff_battery;
Air_diff_to_sum_F_a_feed_forward_controller = [1;1]* O2_ff; 
%%
function feed_forward_tf = make_feed_forward(P,G)
    G = tf(G);
    P = tf(P);


    P_zeros = roots(P.numerator{1});
    P_poles = roots(P.denominator{1});

    is_good_pole = real(P_zeros) <0;
    good_inv_P_unscaled = tf( poly(P_poles), poly(P_zeros(is_good_pole))); % The gain may be wrong

    P_gain = P.numerator{1}(end)/ P.Denominator{1}(end); 
    bad_P_zeros = P_zeros(~is_good_pole); 
    bad_cancelation=  tf( 1, poly(-bad_P_zeros*5e0)); % An attempt to remove the issue (?) 
    inv_P_unscaled = good_inv_P_unscaled * bad_cancelation; 


    if ~isproper(inv_P_unscaled)
    relative_degree = sum(cumprod( (inv_P_unscaled.Denominator{1} == 0))); 
    [a,b] = butter(relative_degree,1e7,"s"); 
    properness_filter = tf(a,b); 
    inv_P_unscaled = inv_P_unscaled*properness_filter; 

    end

    inv_P_gain = inv_P_unscaled.numerator{1}(end)/ inv_P_unscaled.Denominator{1}(end); 
    inv_P = inv_P_unscaled * 1/(P_gain*inv_P_gain);

% 
%     figure 
%     hold on 
%     step(G)
%     step(G - P*inv_P*G)
%     legend("Undamped", "Damped")
    

    feed_forward_tf =inv_P*G;
    
end