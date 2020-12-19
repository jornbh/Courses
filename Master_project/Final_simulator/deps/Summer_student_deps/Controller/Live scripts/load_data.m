% Load simulation output data. Note that the function assumes that specific
% data is logged and in a specific order: [F_aII, F_aI, v_grate, F_w, AB,
% F_O2, F_st, HHV]. If this is not the case, this function will break.
%
% Input
%   cv_name: String determining which CV to load.
%   simOut: SimulationOutput from which the data will be loaded.
% Output
%   y:  Float array with CV response.
%   ty: Float array with CV response time.
%   us: Cell array with float arrays containing input usage.
%   tus: Cell array with float arrays containting input usage time.

function [y, ty, us, tus] = load_data(cv_name, simOut)
    if (cv_name == "O2")
        data = simOut.logsout{6};
    elseif (cv_name == "st" || cv_name == "st_HHV")
        data = simOut.logsout{7};
    elseif (cv_name == "HHV")
        data = simOut.logsout{8};
    elseif (cv_name == "AB" || cv_name == "AB_HHV")
        data = simOut.logsout{5};
    else
        fprintf("Couldn't find %s", cv_name);
    end
    
    y = data.Values.Data;
    ty = data.Values.Time;
    
    us = {};
    tus = {};
    for i=1:4
        input = simOut.logsout{i};
        us{end+1} = input.Values.Data;
        tus{end+1} = input.Values.Time;
    end
    
end
    