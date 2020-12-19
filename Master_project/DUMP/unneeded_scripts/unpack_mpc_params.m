
function unpack_mpc_params(mpc_params_name)


    evalin("caller", sprintf("                mpc_b0 = %s.b0;"                 , mpc_params_name)); 
    evalin("caller", sprintf("              mpc_Linv = %s.Linv;"               , mpc_params_name)); 
    evalin("caller", sprintf("                mpc_Ac = %s.Ac;"                 , mpc_params_name)); 
    evalin("caller", sprintf("mpc_N_prediction_steps = %s.N_prediction_steps;" , mpc_params_name)); 
    evalin("caller", sprintf("           mpc_F_y_ref = %s.F_y_ref;"            , mpc_params_name)); 
    evalin("caller", sprintf("              mpc_F_du = %s.F_du;"               , mpc_params_name)); 
    evalin("caller", sprintf("              mpc_F_x0 = %s.F_x0;"               , mpc_params_name)); 
    evalin("caller", sprintf("                mpc_iA = %s.iA;"                 , mpc_params_name)); 
    evalin("caller", sprintf("          mpc_N_inputs = %s.N_inputs;"           , mpc_params_name)); 

end