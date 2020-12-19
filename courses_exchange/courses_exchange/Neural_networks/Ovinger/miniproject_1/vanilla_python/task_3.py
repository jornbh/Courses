# This will run task 1 if it is tun on a local computer 
# In a notebook, this should not be an issue
try:     
    from initialize_miniproject_1 import *
except: 
    pass



#My code 
########################

def make_model( features,labels, optimizer, learning_rate = None ): 
    # Input layer
    model = Sequential( [Dense(32, input_shape=features[0].shape, batch_size = 128)] )
    model.add(Activation('relu'))

    # Hidden layer
    model.add(Dense(128))
    model.add(Activation('relu'))

    # Output layer
    model.add(Dense(len(labels[0]))) 
    model.add( Activation("softmax"))

    model.compile(
                loss = "categorical_crossentropy", 
                metrics = ["acc"],
                optimizer= optimizer(learning_rate)
                )
    return model

# Variable parameters 

def compare_optimizers(
    features = None, 
    labels = None, 
    learning_rate = None, 
    title = ""

):
    #1)
    SGD_model  = make_model(features,labels, optimizers.SGD,  learning_rate=0.04)
    Adam_model = make_model(features,labels, optimizers.Adam, learning_rate=0.04)
    examples = [
        ("SGD", SGD_model), 
        ("Adam", Adam_model)
    ]
    histories = {}
    for name, model in examples:
        history = model.fit(        features, 
                                    labels,
                                    epochs = GLOBAL_N_EPOCHS,
                                    batch_size = 128,
                                    validation_split = 0.2,
                                )    
        histories[name]= history
    comparison_title = "Task 3 ( {} ) ".format(title)
    comparison_path = "plots/task_3/{}.png".format(title)
    fig = comparison_plot(histories["SGD"], histories["Adam"],"SGD", "Adam", title)
    reckless_savefig(fig, comparison_path)
    
    
def task_3():

    # 3.1 to 3.3
    compare_optimizers(features=x_number_train, labels=y_number_train, learning_rate=0.04, title="First comparison")

    learning_rate_list = np.arange(0.01, 0.1, 0.01)
    learning_sets = [(x_fashion_train, y_fashion_train, "Fashion MNIST"), (x_number_train, y_number_train, "NUMBER MNIST")]



    learning_sets = [
        (x_fashion_train, y_fashion_train, "fashion"),
        (x_number_train , y_number_train , "number"),
    ]
    # Implementation
    for features, labels, name in learning_sets: 
        for learning_rate in learning_rate_list: 
            plot_title = "Task 3 learning_set: {}, learning_rate: {}, epochs: {}".format(name, round(learning_rate,2), GLOBAL_N_EPOCHS )
            compare_optimizers( 
                                features=features,
                                labels=labels,
                                learning_rate=learning_rate,
                                title=plot_title
                            )
task_3()
