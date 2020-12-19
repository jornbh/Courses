# This will run task 1 if it is tun on a local computer 
# In a notebook, this should not be an issue
try:     
    from initialize_miniproject_1 import *
except: 
    pass

# Code to be tested
##############################################################################



def make_model(features, labels, n_hidden_neurons):
    model = Sequential(
        [Dense(32, input_shape=features[0].shape, batch_size=128)])
    model.add(Activation('relu'))

    # Hidden layer
    model.add(Dense(n_hidden_neurons))
    model.add(Activation('relu'))

    # Output layer
    model.add(Dense(len(labels[0])))
    model.add(Activation("softmax"))

    model.compile(
        loss="categorical_crossentropy",
        metrics=["acc"],
        optimizer=optimizers.Adam(0.01)
    )
    return model


def task_4():

        n_hidden_neurons_list = np.linspace(10, 1000, 5).astype(int)
        smallest_losses = []
        training_sets =[
                        (x_number_train,  y_number_train , "number" ),
                        (x_fashion_train, y_fashion_train, "fashion"),
                ]   
        for features, labels, name in training_sets:
                for n_hidden_neurons in n_hidden_neurons_list:
                        model = make_model(features, labels, n_hidden_neurons)
                        model.summary()
                        history = model.fit(
                                                features,
                                                labels,
                                                epochs=GLOBAL_N_EPOCHS, 
                                                batch_size=128, 
                                                validation_split=0.2
                                        )
                        title= "Exercise_4_({} MNIST,_{}_hidden_neurons)".format(name, n_hidden_neurons)
                        fig = plot_history(history, title )
                        reckless_savefig(fig, "plots/task_4/{}.png".format(title))

                        
                        smallest_loss = np.min(history.history["loss"])
                        smallest_losses.append(smallest_loss)

                print(smallest_losses)
                shortest_len = min( len(smallest_losses), len(n_hidden_neurons_list) )
                plt.plot(n_hidden_neurons_list[:shortest_len], smallest_losses[:shortest_len])
                plt.title("Smallest validation loss for different number of hidden neurons "+name)
                plt.xlabel("n layers")
                plt.ylabel("loss")
                reckless_savefig(plt, "./plots/task_4/MNIST_{}_compare_number_of_hidden_nodes.png".format(name))


task_4()
