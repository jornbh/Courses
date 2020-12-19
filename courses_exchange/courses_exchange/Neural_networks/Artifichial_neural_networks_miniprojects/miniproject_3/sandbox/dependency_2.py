from dependency_1 import *
from agent import *
num_episodes = 3000
import numpy as np
# results = dict()
results = Results()
def run_experiment(experiment_name, env, num_episodes, policy_learning_rate = 0.001, value_learning_rate = 0.001, 
                   baseline=None, entropy_cost = 0, max_ent_cost = 0):

    #Initiate the learning agent

    
    agent = RLAgent(n_obs = env.observation_space.shape[0], action_space = env.action_space, # cast env.observation_space to
                    policy_learning_rate = policy_learning_rate, value_learning_rate = value_learning_rate, 
                    discount=0.99, baseline = baseline, entropy_cost = entropy_cost, max_ent_cost = max_ent_cost)

    rewards = []
    all_episode_frames = []
    step = 0
    for episode in range(1, num_episodes+1):
    
        #Update results plot and occasionally store an episode movie
        episode_frames = None
        if episode % 10 == 0:
            results[experiment_name] = np.array(rewards)
            results.plot(10)
        if episode % 500 == 0:
            episode_frames = []
            
        #Reset the environment to a new episode
        observation = env.reset()
        episode_reward = 0

        while True:
        
            if episode_frames is not None:
                pass # The line below might cause a crash, so I removed it 
                #episode_frames.append(env.render(mode='rgb_array'))

            # 1. Decide on an action based on the observations
            action = agent.decide(observation)

            # 2. Take action in the environment
            next_observation, reward, done, info = env.step(action)
            episode_reward += reward

            # 3. Store the information returned from the environment for training
            agent.observe(observation, action, reward)

            # 4. When we reach a terminal state ("done"), use the observed episode to train the network
            if done:
                rewards.append(episode_reward)
                if episode_frames is not None:
                    all_episode_frames.append(episode_frames)
                agent.train()
                break

            # Reset for next step
            observation = next_observation
            step += 1
            
    return all_episode_frames, agent