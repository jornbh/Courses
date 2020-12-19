# Final simulator 
On linux: Run `firefox "README.html"` for prettier documentation. 



## Running the project 
There are several methods of running the project
* Running `Main.m` runs both variations of the PID-controllers, as welll as the different kinds of optimal controllers. 
  * The defalult is to only run the discrete MPC that assumes disturbances on all states, but this can be changed by removing the lines containing the comment: "% Delete these to run more kinds of optimal controllers"
* Running "prepare_furnace.m" adds uses default values for all parameters in the plant and the controller. After that, it should be possible to run the simulink project. 
* Running the script `tune_mpc_battery.m` starts the tuning process for optimizing the controller parameters. This will take at least the entire night. 


## Only runnning simulink 
If the goal is to run the model directly from simulink, run the command `addpath(genpath(pwd));` to include the entire project in the workspace and the script `prepare_furnace.m` to set all variables. Make sure the required blocks were not commented out, and set the parameters. This uses some default values, some of which may be bad. 


## Project Structure 

Each directory is made to make each piece of functionality more independant, so that it may be reused more easily. 

- Main.m (Runs a comparison of all controllers)
- msrc (Matlab source code)
- ssrc (Simulink source code)
- doc (Documentation in HTML)
- deps (Modified project from the previous summer student)
- reqired_data (.mat files needed to make the controllers and estimated models)
- outputs (Directory for all saved results)
<!-- - Main Folder 
  - [identify_system_from_step](doc/identify_system_from_step.html)
  - [integral_based_MPC](doc/integral_based_MPC.html)
  - [automated_tuning](doc/automated_tuning.html) -->



## Notable exported functions
- compare_ERA_approxomations 
  - Compares the different possible model orders when it comes to approxomating a step-response. 