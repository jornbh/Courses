import gym
import tensorflow as tf
import numpy as np
import keras 
import keras.models         as km
import keras.layers         as kl
import keras.optimizers     as ko
import keras.losses         as kloss
import keras.metrics        as kmets
import keras.activations    as kacts

from keras import backend as K
print("\n"*4)
# See if the different modes still work with the neural network
# mode = "normal"
mode = "normal"

# Normal example
if mode == "normal": 
    input_dim = 5
    output_dim = 2
    env = gym.make("NChain-v0")
elif mode == "cart": # This model does not handele discrete spaces.
    input_dim = 4
    output_dim = 2
    env = gym.make('CartPole-v0')

print("Observational space (high): ", env.observation_space.high)
print("Action space", env.action_space)





num_episodes = 20


model = km.Sequential()
model.add(kl.InputLayer(batch_input_shape=(1, input_dim)))
model.add(kl.Dense(10, activation='sigmoid'))
model.add(kl.Dense(output_dim, activation='linear'))
model.compile(loss='mse', optimizer='adam', metrics=['mae'])

model.summary()



# now execute the q learning
y = 0.95
eps = 0.5
decay_factor = 0.999
r_avg_list = []
for i in range(num_episodes):
    s = env.reset()
    eps *= decay_factor
    if i % 100 == 0:
        print("Episode {} of {}".format(i + 1, num_episodes))
    done = False
    r_sum = 0
    while not done:
        if np.random.random() < eps:
            a = np.random.randint(0, output_dim)
        else:
            a = np.argmax(model.predict(np.identity(input_dim)[s:s + 1]))
        new_s, r, done, _ = env.step(a)
        target = r + y * np.max(model.predict(np.identity(input_dim)[new_s:new_s + 1]))
        target_vec = model.predict(np.identity(input_dim)[s:s + 1])[0]
        target_vec[a] = target
        model.fit(np.identity(input_dim)[s:s + 1], target_vec.reshape(-1, output_dim), epochs=1, verbose = 0)
        s = new_s
        r_sum += r
    print("ep done")
    r_avg_list.append(r_sum / 1000)
print(r_avg_list)
plt.plot(r_avg_list)
plt.show()


