function [well_behaved_simout] = set_simout_to_struct(simout_result)
    
    % Sometimes the to workspace block wirtes to the sim() retun-value, and sometimes directly to workspace
    % This script handles both cases
try
    % Try if it is in the struct
    well_behaved_simout.F_O2    = simout_result.simout_F_O2;
    well_behaved_simout.F_st    = simout_result.simout_F_st;

    well_behaved_simout.F_aI    = simout_result.simout_F_aI;
    well_behaved_simout.F_aII   = simout_result.simout_F_aII;
    well_behaved_simout.F_w     = simout_result.simout_F_w;
    well_behaved_simout.v_grate = simout_result.simout_v_grate;
catch
    % If not, nab the results from the caller's workspace
    well_behaved_simout.F_O2    = evalin("caller", "simout_F_O2");
    well_behaved_simout.F_st    = evalin("caller", "simout_F_st");
    well_behaved_simout.F_aI    = evalin("caller", "simout_F_aI");
    well_behaved_simout.F_aII   = evalin("caller", "simout_F_aII");
    well_behaved_simout.F_w     = evalin("caller", "simout_F_w");
    well_behaved_simout.v_grate = evalin("caller", "simout_v_grate");
end

