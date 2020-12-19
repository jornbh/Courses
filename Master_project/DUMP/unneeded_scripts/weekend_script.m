"Start week-end"
disp("Start MPC search")
run search_for_MPC_parameters.m
disp("End MPC search")
save("weekend_MPC", "best_params")
disp("Start PID search")
run search_for_PID_parameters.m
disp("End PID search")
save("weekend_PID", "best_params")

"Week-end complete"

% suffixes = ["F_aI" "F_aII" "F_w_in" "v_grate" "AB" "HHV"];
% for i = 1:length(suffixes)
%     evalin('caller',...
%         sprintf("reg.input_type_%s = InputType.Constant;", suffixes(i)));
% end