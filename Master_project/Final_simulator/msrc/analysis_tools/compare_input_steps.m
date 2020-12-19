close all 
exp_result = load("expanded_disturbance_estimation_experiment_result.mat")
step_responses = exp_result.step_responses;
step_amplitudes = exp_result.step_amplitudes; 
step_time = exp_result.step_time;
Ny = 3; % Number of outputs 
Nu = 4; % Number of inputs 
Nv = 5; % Number of disturbances 
timeseries_start = 0;%exp_result.constant_response{1}.time(1); % First time in the time-series
line_width = 1.5; 
const_outputs = [-1,-1,-1]; 
for i =1:Ny
    const_outputs(i) = exp_result.constant_response{i}.data(end);
end

%% Input step-responses
figure(1)
for i = 1:Ny
    for j = 1:Nu
    subplot(Ny, Nu, (i-1)*Nu + j)
    hold on 
    plot( step_responses{i,j}.time -timeseries_start , step_responses{i,j}.data, "LineWidth", line_width)
    plot(step_time-timeseries_start ,const_outputs(i), "rx" , "LineWidth", line_width)
    hold off
    end
end
output_names = ["F_{st}", "F_{O2}", "HHV"]; 
for i = 1:Ny
    subplot(Ny, Nu, 1 + (i-1)*Nu)
    
    ylabel(output_names(i))
end
input_names = ["F_{aII}", "F_{w,in}", "F_{aI}","v_{grate}"]; 
for i = 1:Nu 
    subplot(Ny, Nu, i)
    title_string = sprintf( "\\Delta %s = %.2d", input_names(i), step_amplitudes{i});
    title(title_string)
    subplot(Ny,Nu, (Ny-1)*Nu +i)
    xlabel("time [s]")
end

sgtitle("Input step-responses")


%% Disturbance step-responses

figure(2)
for i = 1:Ny
    for j = 1:Nv
    subplot(Ny, Nv, (i-1)*Nv + j)
    hold on 
    plot(step_responses{i,j+Nu}.time-timeseries_start,step_responses{i,j+Nu}.data, "LineWidth", line_width)
    plot(step_time-timeseries_start,const_outputs(i), "rx" , "LineWidth", line_width)
    hold off
    end
end
output_names = ["F_{st}", "F_{O2}", "HHV"]; 
for i = 1:Ny
    subplot(Ny, Nv, 1 + (i-1)*Nv)
    ylabel(output_names(i))
end
disturbance_names = [ "T_{aI}"    "T_{aII}", "Q_{grate,1}", "Q_{grate,2}", "Q_{grate,3}"];
for i = 1:Nv
    subplot(Ny, Nv, i)
    title_string = sprintf( "\\Delta %s = %.2d", disturbance_names(i), step_amplitudes{i+Nu});
    title(title_string)
    subplot(Ny,Nv, (Ny-1)*Nv +i)
    xlabel("time [s]")
end

sgtitle("Disturbance step-responses")

%% Normalized differences

% Input step-responses
figure(3)
for i = 1:Ny
    for j = 1:Nu
    
    subplot(Ny, Nu, (i-1)*Nu + j)
    hold on 
    amplitude = step_amplitudes(j);
    current_response = (step_responses{i,j} - const_outputs(i))/amplitude{1};
    xlim([0, current_response.time(end) - current_response.time(1)]);
    plot(current_response.time - step_time, current_response.data, "LineWidth", line_width);
    plot(0,(const_outputs(i)- const_outputs(i)), "rx" , "LineWidth", line_width)
    hold off
    end
end
output_names = ["F_{st}", "F_{O2}", "HHV"]; 
for i = 1:Ny
    subplot(Ny, Nu, 1 + (i-1)*Nu)
    
    ylabel("\Delta " + output_names(i))
end
input_names = ["F_{aII}", "F_{w,in}", "F_{aI}","v_{grate}"]; 
for i = 1:Nu 
    subplot(Ny, Nu, i)
    title_string = sprintf( "\\Delta %s = 1", input_names(i));
    title(title_string )
    subplot(Ny,Nu, (Ny-1)*Nu +i)
    xlabel("\Delta time [s]")
end

sgtitle("Scaled Input step-responses")


%% Disturbance step-responses

figure(4)
for i = 1:Ny
    for j = 1:Nv
    subplot(Ny, Nv, (i-1)*Nv + j)
    amplitude = step_amplitudes(j+ Nu);
    current_response = (step_responses{i,j+Nu} - const_outputs(i))/amplitude {1};
    hold on 
    xlim([0, current_response.time(end) - step_time]);
    
    
    plot(current_response.time - step_time , current_response.data, "LineWidth", line_width)
    plot(0,(const_outputs(i)  - const_outputs(i))/amplitude {1}, "rx" , "LineWidth", line_width)
    hold off
    end
end
output_names = ["F_{st}", "F_{O2}", "HHV"]; 
for i = 1:Ny
    subplot(Ny, Nv, 1 + (i-1)*Nv)
    ylabel("\Delta " + output_names(i))
end

for i = 1:Nv
    subplot(Ny, Nv, i)
    title_string = sprintf( "\\Delta %s = 1", disturbance_names(i));
    title( {title_string ""})
    subplot(Ny,Nv, (Ny-1)*Nv +i)
    xlabel("\Delta time [s]")
end
% git commit -m "Updated the plot function to use difference in time from the time-step instead of time from the stable point"

sgtitle("Scaled disturbance step-responses")


%% Compare HHV to F_st 

figure(5)
for i = 1:Nv
   subplot(Nv,1,i) 
   xlim([0, step_responses{1,i}.time(end)- step_time - 5e3])
   F_st_response = (step_responses{1,i+Nu}  - const_outputs(1));
   HHV_response = (step_responses{3,i+Nu}  - const_outputs(3));
   F_st_percent = 100*F_st_response/F_st_response.data(end);
   HHV_percent = 100*HHV_response/HHV_response.data(end);
   hold on 
   plot(F_st_percent.time - step_time,F_st_percent.data , "LineWidth", line_width)
   plot(HHV_percent.time- step_time,HHV_percent.data , "LineWidth", line_width)
   plot(0 ,0, "rx" , "LineWidth", line_width)
   hold off
   title(disturbance_names(i))
   ylabel("[%]"); 
end
subplot(Nv,1,1)
legend("\Delta F_{st}/ \Delta F_{st}(end)", "\Delta HHV/ \Delta HHV(end)")
sgtitle("F_{st} vs HHV")


%% Compare F_st #TODO


%% Save the plot comparison figures

plot_folder = "outputs/Fig_dump/Simple_analysis_plots/"; 
plot_folder = strrep(plot_folder,"/","\"); 
% saveas(figure(1), plot_folder + "Unscaled_input_step_responses", "epsc"); 
% saveas(figure(2), plot_folder + "Unscaled_disturbance_step_responses", "epsc"); 
% saveas(figure(3), plot_folder + "Scaled_input_step_responses", "epsc"); 
% saveas(figure(4), plot_folder + "Scaled_disturbance_step_responses", "epsc"); 


paper_width = 8; 
paper_height= 8; 

figure(1)
set(gcf, 'PaperUnits', 'inches');
set(gcf, 'PaperSize', [paper_width paper_height]);
set(gcf, 'PaperPositionMode', 'manual');
set(gcf, 'PaperPosition', [0 0 paper_width paper_height]);
print( plot_folder + "Unscaled_input_step_responses", "-depsc"); 
figure(2)
set(gcf, 'PaperUnits', 'inches');
set(gcf, 'PaperSize', [paper_width paper_height]);
set(gcf, 'PaperPositionMode', 'manual');
set(gcf, 'PaperPosition', [0 0 paper_width paper_height]);
print( plot_folder + "Unscaled_disturbance_step_responses", "-depsc"); 
figure(3)
set(gcf, 'PaperUnits', 'inches');
set(gcf, 'PaperSize', [paper_width paper_height]);
set(gcf, 'PaperPositionMode', 'manual');
set(gcf, 'PaperPosition', [0 0 paper_width paper_height]);
print( plot_folder + "Scaled_input_step_responses", "-depsc"); 
figure(4)
set(gcf, 'PaperUnits', 'inches');
set(gcf, 'PaperSize', [paper_width paper_height]);
set(gcf, 'PaperPositionMode', 'manual');
set(gcf, 'PaperPosition', [0 0 paper_width paper_height]);
print( plot_folder + "Scaled_disturbance_step_responses", "-depsc"); 
figure(5)
set(gcf, 'PaperUnits', 'inches');
set(gcf, 'PaperSize', [paper_width paper_height]);
set(gcf, 'PaperPositionMode', 'manual');
set(gcf, 'PaperPosition', [0 0 paper_width paper_height]);
print( plot_folder + "Compare_F_st_to_HHV", "-depsc"); 
"Print done"