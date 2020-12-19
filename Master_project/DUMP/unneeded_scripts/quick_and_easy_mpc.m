function easy_mpc = quick_and_easy_mpc()
%% create MPC controller object with sample time
load("filtered_plant_C");
%% create MPC controller object with sample time
easy_mpc = mpc(filtered_plant_C, 7);
%% specify prediction horizon
easy_mpc.PredictionHorizon = 10;
%% specify control horizon
easy_mpc.ControlHorizon = 10;
%% specify nominal values for inputs and outputs
easy_mpc.Model.Nominal.U = [0;0;0;0];
easy_mpc.Model.Nominal.Y = [0;0];
%% specify constraints for MV and MV Rate
easy_mpc.MV(1).Min = -14;
easy_mpc.MV(2).Min = -4;
easy_mpc.MV(3).Min = -18;
easy_mpc.MV(4).Min = -0.0025;
%% specify constraints for OV
easy_mpc.OV(1).Min = -10.7617914383218;
easy_mpc.OV(2).Min = -0.1;
%% specify overall adjustment factor applied to weights
beta = 2.0544;
%% specify weights
easy_mpc.Weights.MV = [0 0 0 0]*beta;
easy_mpc.Weights.MVRate = [0.001 0.1 0.01 1]/beta;
easy_mpc.Weights.OV = [1 10]*beta;
easy_mpc.Weights.ECR = 100000;
%% specify overall adjustment factor applied to estimation model gains
alpha = 1.8197;
%% adjust default output disturbance model gains
setoutdist(easy_mpc, 'model', getoutdist(easy_mpc)*alpha);
%% adjust default measurement noise model gains
easy_mpc.Model.Noise = easy_mpc.Model.Noise/alpha;
%% specify simulation options
options = mpcsimopt();
options.RefLookAhead = 'off';
options.MDLookAhead = 'off';
options.Constraints = 'on';
options.OpenLoop = 'off';



