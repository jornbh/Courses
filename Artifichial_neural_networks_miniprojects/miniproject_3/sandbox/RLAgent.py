from keras.models import Sequential
import keras.models as models
import keras.layers as layers
from keras.layers import Dense
import keras.backend as K
import numpy as np
class RLAgent(object):

    def __init__(   self, n_obs,
                    action_space,
                    policy_learning_rate,
                    value_learning_rate,
                    discount,
                    baseline = None,
                    entropy_cost = 0,
                    max_ent_cost = 0):
                    a = 10
                    self.n_obs = n_obs 
                    self.action_space = action_space
                    self._build_policy_network()
    def _build_policy_network(self):
        
        inp = layers.Input( shape=(self.n_obs,), # This needs to be a tuple
                            name="input_x" # No idea what this does
                            )
        adv =  layers.Input(shape=[1], name="advantages")



        # Adding 3 hidden layers
        l1 = (Dense(16,activation="relu"))(inp)
        l2 = (Dense(16,activation="relu"))(l1)
        l3 = (Dense(16,activation="relu"))(l2)

        out = layers.Dense(
                    self.action_space, 
                    activation="softmax", 
                    # kernel_initializer=glorot_uniform(seed=42), # Dunno why this is here
                    use_bias=False,
                    name="out")(l3) # THere is a (x) here to add the previous layer here


        model = models.Model(inputs = [inp], outputs = out) # We are suposed to have [inp, adv] as inputs(?) 


        # model = Sequential(inputs= [inp,adv], outputs = out)
        def costum_loss(real_label, network_output):
            log_lik = K.log(real_label * (real_label - network_output) + (1 - real_label) * (real_label + network_output))
            return K.mean(log_lik * adv, keepdims=True)
        model.compile(loss=costum_loss, optimizer="adam")
        self.policy_model= model





if __name__ == "__main__":  
    a = RLAgent(8,3, 0.001, 0.002, 0.9999)
    dummy_input = np.full((2,8), 1)
    predicton =a.policy_model.predict(dummy_input)
    print(predicton)




# # Disfunctional debug 

# class dummy:
#   def __init__(self, n_actions):
#     self.n = n_actions
# learning_rate = 0.1

# agent = RLAgent( 3, dummy(3), 0.001, 0.001, 
#                  0.9999, baseline = None, entropy_cost = 0, max_ent_cost = 0)
# for i in range(100):
#   for j in range(50):
#     state = np.array([0,0, 1 ])
#     action = agent.decide(state)
#     reward = np.sum( state*action)
#     agent.observe(action,state[None, :], reward)
#   rewards = agent.episode_rewards
#   print(sum(rewards))
#   agent.train()