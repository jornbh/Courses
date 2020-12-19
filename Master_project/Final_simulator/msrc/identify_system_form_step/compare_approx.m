%% Run this script to see if the approxomated system behaves rougly correctly
% Need yFull and approxomated_system

%% Set different colours for different model orders
order_colours = zeros(100,3)+1; 

order_colours(5:20,:) = jet(16);
% 
% order_colours(5)  = "r";
% order_colours(7)  = "b";
% order_colours(10) = "m";
% order_colours(12) = "g";

input_names = ["F_{aII} [kg/s]", "F_{w,in} [kg/s]", "F_{aII} [kg/s]", "v grate [m/s]"];
input_names = [input_names, "Q_{grate,1} [J]","Q_{grate,2} [J]", "Q_{grate,3} [J]"]; 
output_names= ["F_{st} [kg/s]", "F_{O2} [kg/s]", "HHV[J/s]"]; % Unsure about HHV (Heat value)
%% Compare impulses
[i_res, i_time] = impulse(approxomated_system);
yFull_time = time(within_experiment); 
is_clean_figure = length(approxomated_system.A) == 15;% (Hack for printing true last)% ~ ishandle(1); % Test if figure exists
figure(1)
Nu = size(approxomated_system.B,2);
Ny = size(approxomated_system.C,1);
figure(Nu*Ny)
for input_no = 1:Nu
    for output_no = 1:Ny
        subplot(Ny,Nu, input_no + Nu*(output_no -1));
        hold on 

        plot(i_time + yFull_time(1), i_res(:,output_no, input_no).*dt, "Color", order_colours(model_order,:), 'LineWidth',2);
        if is_clean_figure
            plot(yFull_time, yFull(:,output_no,input_no), "k--",  'LineWidth',2)
        end
        hold off
%         legend("True", "approx")
%         title("Compare impulses");        
    end
end

for input_no = 1:Nu
       subplot(Ny,Nu, input_no );
       title(input_names{input_no})
       subplot(Ny,Nu, Nu*(Ny-1) + input_no)
       xlabel( "time [s]")
end


for output_no = 1:Ny
       subplot(Ny,Nu, 1 + (output_no-1)*Nu);
       ylabel(output_names{output_no}, 'FontSize',10,'FontWeight','bold','Color','k')
end
sgtitle("Impulse response")

%% Compare step


yFull_time = time(within_difference_experiment); 
[s_res, s_time] = step(approxomated_system, yFull_time - yFull_time(1));

% is_clean_figure = ~ ishandle(2); % Test if figure exists
is_clean_figure = length(approxomated_system.A) == 15;% (Hack for printing true last)


figure(Nu*Ny+1)
for input_no = 1:Nu
    for output_no = 1:Ny
        subplot(Ny,Nu, input_no + Nu*(output_no -1));
        hold on 

        plot(s_time + yFull_time(1), y_star(output_no)+s_res(:,output_no, input_no), "Color", order_colours(model_order,:), 'LineWidth',2);
        if is_clean_figure
            plot(yFull_time, y_star(output_no) + yFull_step(:,output_no,input_no), "k--",'LineWidth',2)
        end
        hold off
%         legend("Real step output", "yFull_{step} + y^*", "approx +y^*")

    end
end

for input_no = 1:Nu
       subplot(Ny,Nu, input_no );
       title(input_names{input_no})
       subplot(Ny,Nu, Nu*(Ny-1) + input_no)
       xlabel( "time [s]")
end

for output_no = 1:Ny
       subplot(Ny,Nu, 1 + (output_no-1)*Nu);
       ylabel(output_names{output_no}, 'FontSize',10,'FontWeight','bold','Color','k')
end
sgtitle("Step response")

% Print the results
if Nu == 4
    sgtitle("Without disturbances") 
    print(gcf, "outputs/Fig_dump/ERA_step_approx_without_disturbance_inputs", "-depsc")
else
    sgtitle("With disturbances") 
    print(gcf, "outputs/Fig_dump/ERA_step_approx_with_disturbance_inputs", "-depsc")
end

"done"; 
%% sigmas

s_diag = diag(Sigma); 

figure 
plot(s_diag./sum(s_diag)); 
xlabel("Number of states") 
xlim([0,30]); 
set(gca, "YScale", "log")

if Nu == 4
    title("Without disturbances") 
    print(gcf, "outputs/Fig_dump/Sigmas_without_disturbances", "-depsc")
else
    title("With disturbances") 
    print(gcf, "outputs/Fig_dump/Sigmas_with_disturbances", "-depsc")
end
