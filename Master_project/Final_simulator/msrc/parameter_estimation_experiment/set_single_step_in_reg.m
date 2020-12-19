function updated_reg = set_single_step_in_reg(old_reg, step_name)
controller_suffixes = ["F_aI" "F_aII" "F_w_in" "v_grate" "AB" "HHV"];

updated_reg = old_reg; 

for current_suffix = controller_suffixes
    eval(sprintf("updated_reg.input_type_%s = InputType.Constant;", current_suffix));
end
eval(sprintf("updated_reg.input_type_%s = InputType.Step;", step_name));
end