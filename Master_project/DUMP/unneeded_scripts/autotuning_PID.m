optimization_vector = [% The initial controller  parameters
1.9769                                   ;
45.8596                                  ;
-1.0000e-03                              ;
2.1723e-07                               ;
12459000                                 ;
-1.0000e-03                              ;
15                                       ;
0.5054                                   ;
0.8579                                   ;
-2.9786e-05                              ;
1.4988e-07                               ;
246910                                   ;
-2.9786e-05                              ;
15                                       ;
1.9769                                   ;
45.8596                                  ;
-1.0000e-03                              ;
2.1723e-07                               ;
12459000                                 ;
-1.0000e-03                              ;
15                                       ;
];
sim_for_controller_cost(optimization_vector)
% attempt = optimization_vector;



function cost = sim_for_controller_cost(attempt)

    % Prepare controller 
    STEP   = [1;       % Primary air
          0;       % Secondary air
          0;       % Grate speed
          0];      % Waste flow
    A_STEP = 1.08;

    run jorns_stolen_parameters.m % Prepare some additional physical parameters
    feedback_type = FeedbackType.AB; 
    simulation_time_end = 8.5*3600 * 10;
    t_STEP = 4*3600;

    % Prepare the controllers
    optimization_vector = attempt; 
    % % Controller prime components 
    reg.Kp_O2     = pid_parameters_vector(1);
    reg.Kp_st     = pid_parameters_vector(2);
    reg.Kp_AB     = pid_parameters_vector(3);
    reg.Kp_HHV    = pid_parameters_vector(4);
    reg.Kp_st_HHV = pid_parameters_vector(5);
    reg.Kp_AB_HHV = pid_parameters_vector(6);
    reg.Kp_fan    = pid_parameters_vector(7);


    % % Controller prime components 
    reg.Ki_O2     = pid_parameters_vector(8);
    reg.Ki_st     = pid_parameters_vector(9);
    reg.Ki_AB     = pid_parameters_vector(10);
    reg.Ki_HHV    = pid_parameters_vector(11);
    reg.Ki_st_HHV = pid_parameters_vector(12);
    reg.Ki_AB_HHV = pid_parameters_vector(13);
    reg.Ki_fan    = pid_parameters_vector(14);


    % %% Derivative componenet 
    reg.Kp_O2     = pid_parameters_vector(15);
    reg.Kp_st     = pid_parameters_vector(16);
    reg.Kp_AB     = pid_parameters_vector(17);
    reg.Kp_HHV    = pid_parameters_vector(18);
    reg.Kp_st_HHV = pid_parameters_vector(19);
    reg.Kp_AB_HHV = pid_parameters_vector(20);
    reg.Kp_fan    = pid_parameters_vector(21);

    % Find a way to test for disturbance

    options = simset('SrcWorkspace','current');
    sim("Furnace_SR2",[],options)
    cost = Inf; 
end



