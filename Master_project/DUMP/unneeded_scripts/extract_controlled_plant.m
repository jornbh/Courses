function [controled_plant] =  extract_controlled_plant(full_plant, controlled_output_indecies,controlled_input_indecies)
%% Returns a Matlab State-space, where only the desired inputs and outputs
% Inputs:
    % full_plant : ss
    % controlled_output_indecies : [integer] | "all"
    % controlled_input_indecies  : [integer] | "all"
% Returns: 
    % controlled_plant : ss 
        
   
%% Default values

    if ~exist("controlled_output_indecies","var") || isequal(controlled_output_indecies,"all")
        controlled_output_indecies = 1:size(full_plant.C,1); 
    end


    if ~exist("controlled_input_indecies","var") || isequal(controlled_input_indecies,"all")
        controlled_input_indecies  = 1:size(full_plant.B,2);
    end

%% Function Body: 

    
    
    % Identity matrices can be multiplied with state spaces to change how the outputs are read
    output_remover_base = eye(size(full_plant.C,1));
    input_remover_base = eye(size(full_plant.B,2));


    
    controled_outptus  = output_remover_base( controlled_output_indecies ,:);
    controled_inputs   = input_remover_base( :, controlled_input_indecies);



    % Now, make the plant the controller sees
    controled_plant = controled_outptus* full_plant * controled_inputs; 
    
end