# This will run task 1 if it is tun on a local computer 
# In a notebook, this should not be an issue
try:     
    from initialize_miniproject_1 import *
except: 
    pass

def make_model(depth, features=None, batch_size=None):
    input_shape = features[0].shape

    model = Sequential([Dense(32, input_shape=input_shape, batch_size=batch_size)])
    for i in range(depth):
        model.add(Activation('relu'))
        model.add(Dense(10))
    model.add(Activation("softmax"))


    model.compile(
        loss="categorical_crossentropy",
        metrics=["acc"],
        optimizer=optimizers.SGD()
    )
    return model

# Get indexes of the samples the model can't learn correctly
def get_mislabeled_indexes(model):
    guesses = model.predict_classes(x_number_test).reshape((-1,))
    ziped = ([(ind, guess) for ind, guess in enumerate(guesses)])

    label_attempts = np.array([y_test[index] for index in ziped])
    is_mislabeled = True != label_attempts

    mislabeled_indexes = np.arange(is_mislabeled.shape[0])[is_mislabeled]

    return mislabeled_indexes, guesses


def task_2():  # Keep the momory from blowing up
    fig = plot_some_samples(x_number_train, y_number_train)
    reckless_savefig(fig, "plots/task_2/Task_2_Some_arbitrary_numbers.png")



    # Testing the networks

    # The cases we want to train on: (features, labels, model-name)
    training_cases = [ 
        (x_number_train , y_number_train        , "number"  ),  
        (x_fashion_train, y_fashion_train, "fashion" ),  
    ]

    for features, labels, name in training_cases: 
        batch_size = 128
        model = make_model(2, features=features, batch_size=batch_size)
        history = model.fit( 
                                    features, 
                                    labels,
                                    epochs=GLOBAL_N_EPOCHS,
                                    batch_size=batch_size,
                                    validation_split=0.2
                                )

        # Plot history
        history_path = "plots/task_2/Task_2_{}.png".format(name)
        fig = plot_history(history, "Task_2 {}".format(name) )
        reckless_savefig(fig, history_path)
        mislabeled_indexes, guesses = get_mislabeled_indexes(model)

        # Plot mislabeling
        mislabeled_path = "plots/task_2/Task_2_Some_mislabeled_{}_samples_after_training.png".format(name)
        fig = plot_some_samples(features, labels, guesses, mislabeled_indexes)
        reckless_savefig(fig, mislabeled_path)


task_2()