# This will run task 1 if it is tun on a local computer 
# In a notebook, this should not be an issue
try:     
    from initialize_miniproject_1 import *
except: 
    pass

    
def log(*args, file= None, **kwargs): 
    try:
        if log_file == None: 
            log_file = open("logs/task_6/main_log", "a") 
            print(*args, **kwargs, file=log_file)
            log_file.close()
        else: 
            print(*args, **kwargs, file=log_file)
    except: 
        print("Unable to write to log")
        print(*args, **kwargs)

log("#"*26, "Starting new run of task 6", sep="\n")

# Convolutional neural networks

def make_one_convolution_model(training_set, label_set): 
    # One convolution, two layers
    learning_rate = 0.004
    # n_labels = label_set.shape[1]
    num_classes = 10

    # Single later, but several convolutional layers
    output_model = Sequential()
    output_model.add(Conv2D(32*2, kernel_size=(4, 4),activation='linear',input_shape=(28,28,1),padding='same'))
    output_model.add(Activation("relu"))
    output_model.add(MaxPooling2D((4, 4),padding='same')) # I use max-polling because I decide to be stupid and not listen to others

    output_model.add(Flatten()) # We need a flattening layer (?) 
    output_model.add(Dense(128, activation='linear'))
    output_model.add(Activation("relu"))  
    output_model.add(Dense(128, activation='linear'))
    output_model.add(Activation("relu"))                   
    output_model.add(Dense(num_classes, activation='softmax'))

    output_model.compile( 
                    loss= "categorical_crossentropy",
                    metrics=["acc"],
                    optimizer=keras.optimizers.Adam(learning_rate)
    )
    output_model.summary()
    return output_model


def make_many_convolutions_model(training_set, label_set, learning_rate = 0.004): 
    # n_labels = label_set.shape[1]
    num_classes = 10

    # Single later, but several convolutional layers
    output_model = Sequential()
    output_model.add(Conv2D(32, kernel_size=(3, 3),activation='linear',input_shape=(28,28,1),padding='same'))
    output_model.add(Activation("sigmoid"))
    output_model.add(MaxPooling2D((2, 2),padding='same')) # I use max-polling because I decide to be stupid and not listen to others
    output_model.add(Conv2D(64, (3, 3), activation='linear',padding='same'))
    output_model.add(Activation("sigmoid"))
    output_model.add(MaxPooling2D(pool_size=(2, 2),padding='same'))
    output_model.add(Conv2D(128, (3, 3), activation='linear',padding='same'))
    output_model.add(Activation("sigmoid"))                  
    output_model.add(MaxPooling2D(pool_size=(2, 2),padding='same'))
    output_model.add(Flatten())
    output_model.add(Dense(128, activation='linear'))
    output_model.add(Activation("sigmoid"))                  
    output_model.add(Dense(num_classes, activation='softmax'))

    output_model.compile( 
                    loss= "categorical_crossentropy",
                    metrics=["acc"],
                    optimizer=keras.optimizers.Adam(learning_rate)
    )
    output_model.summary()
    return output_model

def task_6_1(
    learning_x = x_number_train_raw[:,:,:,None],
    learning_y = y_number_train, 
    training_set_name = "MNIST"
): 

    my_n_epochs = GLOBAL_N_EPOCHS*2

    # Test the different models
    many_convolutions_model = make_many_convolutions_model(learning_x, learning_y)
    one_convolution_model = make_one_convolution_model(learning_x, learning_y)
    
    log("Start training the many convolutions net")
    many_convolutions_history = many_convolutions_model.fit(learning_x, learning_y, epochs = my_n_epochs, batch_size=DEBUG_BATCH_SIZE, validation_split=0.2)
    fig = plot_history(many_convolutions_history, "Task 6 "+training_set_name+" (Many convolutions) ")
    reckless_savefig(fig, "plots/task_6/"+training_set_name+"many_convolutions_network.png")
    # fig.savefig("plots/task_6/many_convolutions_network.png")
    
    plt.close(fig)
    log("Start training the one convolution, two layer net")

    one_convolution_history = one_convolution_model.fit( learning_x, learning_y, epochs = my_n_epochs, batch_size=DEBUG_BATCH_SIZE, validation_split=0.2)
    fig = plot_history(one_convolution_history, "Task 6 "+training_set_name+" (One convolution, 2 hidden layers)")
    reckless_savefig(fig, "plots/task_6/"+training_set_name+"__one_convolution_network.png")







if __name__ == "__main__":
    task_6_1(
        learning_x          =x_number_train_raw[:,:,:,None],
        learning_y          =y_number_train,
        training_set_name   ="MNIST_"
    )

    task_6_1(
        learning_x         = x_fashion_train_raw[:,:,:,None],
        learning_y         = y_fashion_train,
        training_set_name  ="Fashion_MNIST"
    )