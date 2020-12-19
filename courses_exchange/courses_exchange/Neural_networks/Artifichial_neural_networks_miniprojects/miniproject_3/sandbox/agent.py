try:
    from dependency_1 import * # Imported by 2
    from dependency_2 import *
    from my_dependencies import *

except Exception as e:
    print(e)
    print("Did not import dependencies.py (It just emulates some of the variables in the notebook")
    print("In the notebook it is correct that it is not suposed to be imported")

from base_agent import *



class RLAgent_with_entropy(RLAgent_with_adaptive_baseline):
    def __init__(self, *args, **kwargs):
        RLAgent_with_adaptive_baseline.__init__(self, *args, **kwargs)
    
    def make_baseline_fun(self): 
        output_shape = self.model.layers.output_shape 
        


RLAgent =RLAgent_with_adaptive_baseline



# RLAgent = RLAgent_w_simple_baseline 

if __name__ == "__main__":
        
    # Temporary define RLAgent as this

    # Debug (Can be deleted later)




    # I made a dummy env that thests for argmax => The learner is awfull
    class dummy_obs_space:
        def __init__(self,n):
            self.shape = (n,)
            self.n = n
    class dummy_env:
        def reset(self):
            self.is_done =0
            return np.ones(4)
        def __init__(self):
            self.is_done = 0
            self.action_space = dummy_obs_space(3)
            self.observation_spaces= [dummy_obs_space(4)]
            self.observation_space = dummy_obs_space(4)
            
        def step(self, action):
            self.is_done +=1
            return np.ones(4), action*1.0, self.is_done >= 100, ""
            # observation, reward, done, info
            # return np.argmax(action)

    dum = dummy_env()
    print( dum.step(1))
    
    results = Results()
    value_learning_rate=0.1
    policy_learning_rate = 0.1
    #_, _ =        run_experiment("REINFORCE",                dummy_env(), 1000, policy_learning_rate)
    episodes, _ = run_experiment("REINFORCE (with baseline)", dummy_env(), 1000, policy_learning_rate, baseline='adaptive')



    # The learner is quite terrible at the moment, I tested with one I took from the internet as well, and it performesd just as poorly