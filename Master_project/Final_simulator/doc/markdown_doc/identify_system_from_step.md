# Identify system from input steps
Set of functions for system identification, using the step-responses of an unknown plant. 


## Exports

* Step_method_main
* ERA



## Step_method_main
Takes constant and step-responses to produce a discrete linear model of the plant with m outputs and n inputs.


### Arguments: 
* constant_response  
  * [timeseries object] : Response with no change in inputs. (Uniform time-steps not required)
* step_responses     
  * [$m \times n$ cell array of timeseries objects] : Index [i,j] is the response on output i to a step at input j. 
* step_amplitudes    
  * [$n$-dimensional cell array ]
* step_time
  * [Floating point number          ] : Time at which the input-steps occur
* model_order
  * [Integer        ] : Desired order of the returned model
* u_star   
  * [Floating point array           ] :  Array, representing the constant input given to the model. 
* dt                 
  * [Floating point number] : Length of the discrete time-step in the estimated model. 


### Returns
* approxomated_system
  * [state-space object] : LTI-model that matches an assumed impulse-response as well as possible
* sigmas
  * [array] : Contributin from each additional state, as described in the thesis, or in the original ERA-source.
* tau_filter
  * [Floating point number] : A low-pass filter is needed to prevent aliasing when estimating the model. That filter is $1/(\tau_{filter}s +1)$ 


## ERA
Lower-level function used for making an approxomated LTI system, 3-dimensional matrix representing time-series with n inputs, m outputs, over N time-steps. The normal operating point of the inputs and outputs have to be 0.

### Arguments:  
* yFull
  * [ $N \times m \times n$ matrix] : Element [i,j,k] should represent the response of output j, at time-step i to a step at input k. 
* r
  * [Integer] : Order of the returned model. 
* dt 
  * [Float] : The uniform difference in time between each sample

### Returns:
* approx_sys
  * [Discrete state-space object] : Approxomated system
* sigmas
  * [Float array] : Contribution of each state, as described in the ERA-source
* U
  * [Float matrix] : $U$ from the SVD used in the ERA
* Sigma 
  * [Float matrix] : $\Sigma$ from the SVD used in the ERA
* V
  * [Float matrix] : $V$ from the SVD used in the ERA

## Bonus features
* Uncomment the line "run compare_approx" to see the comparison between the real step-response and the approxomated one
  * The script also saves the figure as a coloured eps vetor picture. 
