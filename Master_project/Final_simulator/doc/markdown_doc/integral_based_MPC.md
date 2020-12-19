# Integral-based MPC 
Set of functions making MPC, LQR and LQE parameters, given models and a set of weights. All controllers have integral action on the outputs to prevent constant disturbances. 

## Exports 
* Make_lqe
* make_MPC_lqr_style
* make_MPC_diff_style


## Make_lqe
Makes a kalman-filter, corresponding to a set of weights. A kalman-filter combines noisy measurements with an imperfect model to estimate the system states. The plant is assumed to have $n$ measurements and $m$ disturbances. 

### Arguments
* plant
  * [Contineous LTI-state space model] : The estimated plant
* Q_lqe
  * [$n \times n$] : Co-variance of measurement noise. Larger numbers means trusting the measurements less. 
* R_lqe
  * [$m \times m$ matrix]: Co-variance of plant disturbances. Larger numbers means trusting the model less. 
* G
  * [$N \times m$ matrix] : Input matrix from distrubances to the plant. Default value is $I$, which means all states are affected by distrubances. 
* tau_anti_aliasing_filter
  * [Float] : A filter was required when approxomating the plant. This filter has to be added to the simulink model for proper state estimation. 

### Returns
* lqe_params
  * [struct]
    * K_lqe
      * [$N \times n$ matrix] : The lqe feedback matrix
    * observed_plant
      * [contineous state-space object]
    * anti_aliasing_filter
      * [$n \times n$ contineous state-space] : All mesurements must first be filtered by this filter to ensure correct behaviour. 
    * tau_anti_aliasing_filter
      * [Float] : Time-constant of the input-filter
    * G_lqe
      * [$N \times m$ matrix] : Matrix allowing measured disturbances affect the state estimate. (Allows for better performance with less aggressive estimators)
    <!-- * #TODO complete this part -->



## make_MPC_lqr_style

Makes all the required parameters for making an MPC with integral and derivative action on the outputs. Parameters for making an LQR are also included. The plant has $n_y$ outputs, $n_u$ inputs and $n_x$ states. A filter must be added to the plant to ensure an MPC is able to controll it. 

### Arguments 
* controlled_plant_c
  * [contineous state-space object] : LTI system, with included inputs. The oututs are only the ones that are suposed to be controlled. The integral states are added by the function
* Qy
  * [$2n_y \times 2n_y$] : Weights to normal and integral states. The integra states are the bottom n states used in $y^T Q_y y$
* Qyd
  [$n_y \times n_y$]* Cost of changes in the oututs. 
* R
  * [$n_u \times n_u$ matrix] : Cost of using the inputs
* Rd
  * [$n_u \times n_u$ matrix] : Cost of changing the inputs 
* N_prediction_steps
  * [Integer] : Number of steps the discrete MPC looks ahead.
* MPC_dt
  * [Float] : The time-steps used when discretizing the plant. 

### Returns
* mpc_params
  * [struct] : All required parameters needed to implement an MPC or an LQR in simulink if a full-state estimate is available
    * Linv
      * [$(N_{\text{prediction steps}} \cdot n_u) \times (N_{\text{prediction steps}} \cdot n_u)$] : Pre-processed version of the matrix used in the MPC-cost
    * F_y_ref
      * [$(N_{\text{prediction steps}} \cdot n_u) \times n_y$] : Linear term in the MPC-cost dependant on the reference.
    * F_x0
      * [$(N_{\text{prediction steps}} \cdot n_u) \times n_x$] : Linear term in the MPC-cost taking into account the initial state.
    * F_du
      * [$(N_{\text{prediction steps}} \cdot n_u) \times n_u$] : Linear term in the MPC-cost dependant on the previously used input
    * K_lqr
      * [$n_u \times n_x$] : Feedback matrix for the LQR
    * reference_B
      * [$n_x \times n_y$] : Used for making the integral state increase if the reference is non-zero
    * MPC_plant
      * [discrete state-space object] : The complete, discretized plant the MPC acts on.
    * Ac
      * [N$_{\text{constraints}} \times (n_u \cdot N_{\text{prediction steps}})$ matrix] : The A matrix for the inequality-constraints $A_c \cdot U \geq b_0 + x_{0,corr} x(0)$ if $U$ are all inputs within the time-horizon and $x(0)$ is the state right now, 
    * b0
      * [$N_{\text{constraints}} \times 1$] : Same, but the b-matrix
    * x0_corr
      * [$N_{\text{constraints}} \times n_x$] : Allows for hard constraints on the states, in addition to hard constrints on the inputs. 
    * mpc_function_params
      * [struct]
        *  Linv, F_y_ref, F_x0, F_du, Ac, b0, x0_corr
           *  Required parameters for a Matlab function block in Simulink, implementing an MPC
    * mpc_function_parameter_type
      * Simulink requires the types of each element of `mpc_function_params` to be specified. 
    * controlled_plant
      * [contineous state-space] : The plant the MPC and LQR beleive they are controlling, including the required input-filter
    * input_filter
      * [contineous state-space] : Required filter on the input-signals, so that the controlled plant is strictly proper and controlable by an MPC ($n_u$ inputs and outputs). 
    * N_refs
      * [Integer] : Number of controlled outputs
    * N_inputs
      * [Integer] : Number of plant inputs
    * N_prediction_steps
      * [Integer] : Number of prediction steps used by the plant
    * iA
      * [$N_{\text{constraints}} \times 1$ matrix] : Boolean variables representing which inequalty constraints were active last MPC iteration, speeding up simulation.  
    * K_lqr_d
      * [$n_u \times n_x$ matrix] : Discrete LQR feedback matrix


## make_MPC_diff_style
Exactly the same as `make_MPC_lqr_style`, except for that `make_MPC_lqr_style` uses the input-filter to make am estimate of the change in input, when calculating the cost relating to $R_d$. `make_MPC_diff_style` uses the difference between each iteration and each time-step. This means that `make_MPC_lqr_style` is more in line with the infinite-horizon approxomation, while `make_MPC_diff_style` is more accurate to what the difference actually is suposed to represent. The arguments and returned variables are the same. 

