
try:
    from dependency_1 import * # Imported by 2
    from dependency_2 import *
    from my_dependencies import *

except Exception as e:
    print(e)
    print("Did not import dependencies.py (It just emulates some of the variables in the notebook")
    print("In the notebook it is correct that it is not suposed to be imported")

try:
    from dependency_1 import * # Imported by 2
    from dependency_2 import *
    from my_dependencies import *

except Exception as e:
    print(e)
    print("Did not import dependencies.py (It just emulates some of the variables in the notebook")
    print("In the notebook it is correct that it is not suposed to be imported")

# The net does not train correctly at all
class RLAgent_base(object):
    
    def __init__(self, n_obs, action_space, policy_learning_rate, value_learning_rate, 
                 discount, baseline = None, entropy_cost = 0, max_ent_cost = 0):

        #We need the state and action dimensions to build the network
        self.n_obs = n_obs  
        self.n_actions = action_space.n      
        self.gamma = discount
        self.policy_learning_rate = policy_learning_rate #! 
        self.value_learning_rate = value_learning_rate #! 
        
        
        # self.use_baseline = (baseline =="adaptive" or baseline == "simple") #! 
        # self.use_adaptive_baseline =  (baseline == "adaptive") #! 

        #These lists stores the cumulative observations for this episode
        self.episode_observations, self.episode_actions, self.episode_rewards = [], [], []
        
        #Build the keras network
        self._build_network()
    
    
    def observe(self, state, action, reward):
        self.episode_observations.append( state)
        self.episode_actions.append(      action)
        self.episode_rewards.append(      reward)
    
    # Virtual functions that can be overwritten
    def update_baseline(self):
        pass 
    def get_baselines(self, observations):
        return np.zeros(len(observations))

        
    def decide(self, state):

        distribution = self.model.predict(state[None, : ]) # This is made for multiple timesteps, but we only have one

        possible_actions =  np.arange( 0, distribution.shape[1])
        probabilities = distribution[0]
        chosen_action = np.random.choice(possible_actions, p=probabilities) # Choose one random index based on the distribution 
        
        
        return chosen_action

    def train(self):  
        observations = np.array(                   self.episode_observations   )
        rewards      = np.array(                   self.episode_rewards        )
        actions      = keras.utils.to_categorical( self.episode_actions     ,num_classes=self.n_actions   )

        discounted_rewards = self._get_returns()
  
        # self.model.fit( observations, actions, sample_weight= discounted_rewards, verbose=0)
        # self.model.fit(x=observations, y=[actions, discounted_rewards],)
        self.training_function(  [observations,actions, discounted_rewards] )
        print("#", end="") # To show the progress
        
        self.update_baseline()

        # Forget all previous observations
        self.episode_observations, self.episode_actions, self.episode_rewards = [], [], []
        
        
        
    def _get_returns(self):
        rewards = np.array(self.episode_rewards)
        baselines = self.get_baselines(np.array(self.episode_observations))
        adjusted_rewards = rewards- baselines
        return self.discount_rewards(adjusted_rewards)
      
    def discount_rewards(self, adjusted_rewards):    
        cum_rewards = np.zeros(adjusted_rewards.shape)
        cum_rewards[-1] = adjusted_rewards[-1]
        for ind in range(adjusted_rewards.shape[0]-1, 0, -1):
          cum_rewards[ind-1] = adjusted_rewards[ind-1] + self.gamma*cum_rewards[ind]
        return cum_rewards

      
    def _build_network(self):

        inp = (keras.layers.Input(shape=(self.n_obs,),name="input_x"))
        h1 =(Dense(16, activation="sigmoid")(inp))
        h2 =(Dense(16, activation="relu") )(h1)
        h3 =(Dense(16, activation="relu") )(h2)
        out=Dense(self.n_actions, activation="softmax", use_bias=False,name="out")(h3)
        
        model = keras.models.Model( inputs= [inp], outputs= out)
        # We need a rule that lets us do reward*discount * grad_theta( ln( pi( a | t, theta)))
        # This compile part might have to be changed
        # Categorical cross entropy, together with saving all action-probabilities will let us do that 
        # The theory seems to make somewhat sense now, but I still don't know how to implement it (This might give some hints) 
        # https://gist.github.com/kkweon/c8d1caabaf7b43317bc8825c226045d2
        self.model = model
        # self.train_function = self.make_training_function(model)
        self.training_function = self.make_training_function(self.model)
        # loss_fun= self.make_costum_loss_function(model)
        # self.model.compile(optimizer=Adam(lr=self.policy_learning_rate), loss=loss_fun, )

        # self.model.compile(optimizer=Adam(lr=self.policy_learning_rate), loss="categorical_crossentropy", )
    
        


    def make_costum_loss(self, model,with_placeholders = False):
        network_output_placeholder = model.output
        # The none is there because we can take in an unknown number of observations->rewards
        output_shape = model.layers[-1].output_shape
        #print("OS", output_shape)
        real_action_placeholder     = K.placeholder(shape = output_shape,
                                                    name = "real_action"
                                                    )
        discount_reward_placeholder = K.placeholder(shape=(None,), 
                                                    name="discount_reward"
        )                                                     

        # We use sum because I'm not sure of all the functionality in k
        p_action = K.sum( real_action_placeholder* network_output_placeholder , axis = 1) # The real one is 1-hot encoded, so this will extract it
        log_prob = K.log(p_action)
        loss_mean_elements = - log_prob * discount_reward_placeholder
        loss = K.mean(loss_mean_elements)
        # loss_fun = K.function( [ model.input,real_action_placeholder, discount_reward_placeholder ] ,[])
        if with_placeholders:
            return loss, network_output_placeholder, real_action_placeholder, discount_reward_placeholder
        else:
            return loss
        # return loss_fun
    def make_training_function(self, model):
        loss,network_output_placeholder, action_placeholder, discount_reward_placeholder = self.make_costum_loss(model, with_placeholders=True)
        adam_optimizer = keras.optimizers.Adam(lr= self.policy_learning_rate)
        updates = adam_optimizer.get_updates(
                                    params= model.trainable_weights,
                                    loss=loss
                                )
        training_function=  K.function(inputs=[
                                                    model.input, 
                                                    action_placeholder, 
                                                    discount_reward_placeholder
                                                    ], 
                                            outputs = [], 
                                            updates= updates
                                        )
        return training_function 
      
      
class RLAgent_w_simple_baseline(RLAgent_base):
    def __init__(self, *args,  baseline=None, **kwargs, ):
        RLAgent_base.__init__(self, *args, **kwargs, baseline= baseline)
        
        # Init for handling the moving average baseline
        if baseline == "simple":
            self.moving_average_baseline         = 0
            self.moving_average_ringbuffer       = np.zeros(20) # Moving average of the last 20 samples
            self.moving_average_ringbuffer_index = 0

            # Replace the functions with the correct ones
            self.update_baseline = self.simple_update_baseline
            self.get_baselines = self.get_moving_window_average_baselines
            
        
    def simple_update_baseline(self):
            # Update moving average baseline
        self.moving_average_baseline += np.mean(self.episode_rewards)
        self.moving_average_baseline -= self.moving_average_ringbuffer[self.moving_average_ringbuffer_index]
        self.moving_average_ringbuffer_index = (self.moving_average_ringbuffer_index+1) % len(self.moving_average_ringbuffer)
        print("U",self.moving_average_baseline )
    def get_moving_window_average_baselines(self, observations):
        return np.full( len(observations), self.moving_average_baseline)





class RLAgent_with_adaptive_baseline(RLAgent_w_simple_baseline):
    def __init__(self, *args, baseline= None, **kwargs):
      print(args)
      print(kwargs)
      RLAgent_w_simple_baseline.__init__(self, *args, baseline = baseline, **kwargs) # Handles everything except the "adaptive case"
      
      

      if baseline == 'adaptive': # We might need one for when we have two networks as well
        self.get_baseline = self.get_adaptive_baselines
        self.update_baseline = self.update_adaptive_baseline
        # Make the critic
        self.make_critic()
        self.log = [] # So that we can say something about the values later
    def make_critic(self):
        # Except for the number of outputs, it is to the policy 

        
        self.critic = Sequential()
        self.critic.add(Dense(16, activation="sigmoid", input_shape = (self.n_obs,)) )
        self.critic.add((Dense(16, activation="relu") ))
        self.critic.add((Dense(16, activation="relu") ))
        self.critic.add(Dense( 1, use_bias=True))
        opt = keras.optimizers.Adam(lr=self.value_learning_rate)
        self.critic.compile( optimizer=opt, loss="mse" )
    def train(self, *args, **kwargs):
        if len(self.log) < 10**6:
          self.log += self.episode_observations
        else:
          self.log = self.log[-10**4:] + self.episode_observations # Keep only the newest(Most relevant)
        self.log = self.episode_observations
        RLAgent_w_simple_baseline.train(self,*args,**kwargs)
    def get_adaptive_baselines(self, observations):
      return self.critic.predict(observations)
    def update_adaptive_baseline(self): # The value network does nothing but predict 0
        print("/", end="")
        rewards = np.array(self.episode_rewards)
        discounted_rewards = self.discount_rewards( rewards)

        observations = np.array( self.episode_observations )
        # DEBUG
        # old_predictions = self.critic.predict(observations)
        
        self.critic.fit(observations, discounted_rewards, verbose=0)
        
        # new_predictions = self.critic.predict(observations)
        # print("S",np.abs( new_predictions- discounted_rewards[0]).shape)
        # print("C")
        # for  new, old in zip( np.abs( new_predictions) , np.abs( new_predictions)):
        #   print("(",new, old, "=>", new- old, ")",end="")
        



RLAgent = RLAgent_w_simple_baseline 

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
   # episodes, _ = run_experiment("REINFORCE (with baseline)", dummy_env(), 1000, policy_learning_rate, baseline='adaptive')

    # The learner is quite terrible at the moment, I tested with one I took from the internet as well, and it performesd just as poorly



RLAgent = RLAgent_w_simple_baseline 

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