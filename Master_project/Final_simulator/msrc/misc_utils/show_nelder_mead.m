x = [1,1;-1,-1; 0.5,-0.5];
x1 = x(1,:);
x2 = x(2,:) ;
x3 = x(3,:);
x_bar = (x1 + x2) /2;


point_width = 2; 
line_width = 1; 

%% Initial 
figure 
hold on
plot(x(:,1), x(:,2), "ko", "LineWidth", point_width)
plot(x(1:2,1), x(1:2,2), "k--",  "LineWidth", line_width)
plot(x_bar(1),x_bar(2), "bo","LineWidth", point_width)

x_labels = {"x_1", "x_2", "x_3"}
text(x(:,1), x(:,2), x_labels, 'VerticalAlignment','top','HorizontalAlignment','left')

xlim([min(x(:,1)), max(x(:,1))].*1.1)
ylim([min(x(:,2)), max(x(:,2))].*1.2)


%% Reflext 
subplot(2,2,1)
hold on
% Plot lines
x_new = x_bar + 1*(x_bar - x3); 
plot([x3(1),x_new(1)], [x3(2),x_new(2)], "k--",  "LineWidth", line_width)
plot(x(1:2,1), x(1:2,2), "k--",  "LineWidth", line_width)

plot(x(:,1), x(:,2), "ko", "LineWidth", point_width)

plot(x_bar(1),x_bar(2), "bo","LineWidth", point_width)

x_labels = {"x_1", "x_2", "x_3"}
text(x(:,1), x(:,2), x_labels, 'VerticalAlignment','top','HorizontalAlignment','left')

plot(x_new(1), x_new(2), "ro","LineWidth", point_width);
text(x_new(1), x_new(2), "x_{new}", 'VerticalAlignment','top','HorizontalAlignment','left')
xlim([min(x(:,1)), max(x(:,1))].*1.1)
ylim([min(x(:,2)), max(x(:,2))].*1.2)

title("Reflect")


%% Expand 
subplot(2,2,2)
hold on
% Plot lines
x_new = x_bar + 2*(x_bar - x3); 
plot([x3(1),x_new(1)], [x3(2),x_new(2)], "k--",  "LineWidth", line_width)
plot(x(1:2,1), x(1:2,2), "k--",  "LineWidth", line_width)

plot(x(:,1), x(:,2), "ko", "LineWidth", point_width)

plot(x_bar(1),x_bar(2), "bo","LineWidth", point_width)

x_labels = {"x_1", "x_2", "x_3"}
text(x(:,1), x(:,2), x_labels, 'VerticalAlignment','top','HorizontalAlignment','left')

plot(x_new(1), x_new(2), "ro","LineWidth", point_width);
text(x_new(1), x_new(2), "x_{new}", 'VerticalAlignment','top','HorizontalAlignment','left')
xlim([min(x(:,1)), max(x(:,1))].*1.1)
ylim([min(x(:,2)), max(x(:,2))].*1.2)

title("Expand")

%% Contract 
subplot(2,2,3)
hold on
% Plot lines
x_new = x_bar + 0.5*(x_bar - x3); 
plot([x3(1),x_new(1)], [x3(2),x_new(2)], "k--",  "LineWidth", line_width)
plot(x(1:2,1), x(1:2,2), "k--",  "LineWidth", line_width)

plot(x(:,1), x(:,2), "ko", "LineWidth", point_width)

plot(x_bar(1),x_bar(2), "bo","LineWidth", point_width)

x_labels = {"x_1", "x_2", "x_3"}
text(x(:,1), x(:,2), x_labels, 'VerticalAlignment','top','HorizontalAlignment','left')

plot(x_new(1), x_new(2), "ro","LineWidth", point_width);
text(x_new(1), x_new(2), "x_{new}", 'VerticalAlignment','top','HorizontalAlignment','left')
xlim([min(x(:,1)), max(x(:,1))].*1.1)
ylim([min(x(:,2)), max(x(:,2))].*1.2)

title("Contract")

%% Shrink 
subplot(2,2,4) 
hold on
% Plot lines
x_new = x_bar - 0.5*(x_bar - x3); 
plot([x3(1),x_new(1)], [x3(2),x_new(2)], "k--",  "LineWidth", line_width)
plot([x3(1),x_bar(1)], [x3(2),x_bar(2)], "k--",  "LineWidth", line_width)
plot(x(1:2,1), x(1:2,2), "k--",  "LineWidth", line_width)

plot(x(:,1), x(:,2), "ko", "LineWidth", point_width)

plot(x_bar(1),x_bar(2), "bo","LineWidth", point_width)

x_labels = {"x_1", "x_2", "x_3"}
text(x(:,1), x(:,2), x_labels, 'VerticalAlignment','top','HorizontalAlignment','left')

plot(x_new(1), x_new(2), "ro","LineWidth", point_width);
text(x_new(1), x_new(2), "x_{new}", 'VerticalAlignment','top','HorizontalAlignment','left')
xlim([min(x(:,1)), max(x(:,1))].*1.1)
ylim([min(x(:,2)), max(x(:,2))].*1.2)

title("Shrink")

%% Print the figure 
print("JornDir\Fig_dump\Show_Nelder_Mead", gcf, "-depsc")