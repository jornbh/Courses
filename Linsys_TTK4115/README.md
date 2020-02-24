# Linsys_TTK4115
A basic guide tht will (Hopefully) make it easier to use plots in Matlab

## TL;DR
* Use the to file block in simulink to get data to a .mat file 
* load('filename') loads the data from a .mat file to the matlab workspace 
* print('filename', '-depsc') saves the plot that is currntly displayed as in eps-format with colours

## Running the guide
Open guide.m in the top folder
1. Run the entire script first (Or just run the init-section)
2. Find a plot that has something you need
3. The section corresponding to  (ctrl+f) to find the section ( It has the same name as the title of the plot)
4. To only run one section hit (ctrl+enter)
    * The current section is marked in yellow  your corsor is above it 
    * The Plots can be run independently, as long as the init-part has been run





### Misc. Matlab-tips
* close all
    * Closes all plots
* sim('simulink_filename')
    * Runs a simulink-simulation. The filename is without the .slx - ending 
    *  Does not build anything
* Use a to file-block to get data from a simulink-simulation to a .mat -file   
     ![Picture missing](Resources/to_file_block.png)
    * Remember to change the output-type to array
     ![Picture missing](Resources/Change_to_array.png)
* load('filename') 
    * Get the variable(s) from the .mat file to the workspace (filename is without .mat)
    * The first row of the matrix is time, the rest are in order of input.

* save filename
    * Lets you save the current workspace as filename.mat
    * Can be called without any parenthesis or quotation-marks

### Not implemented:
* Titles in Plots with multiple subplots
* Rotating the y-label and changing its position
* Adding LaTex-code to the labels and legends

