# This will run task 1 if it is tun on a local computer 
# In a notebook, this should not be an issue
try:     
    from initialize_miniproject_1 import *
except: 
    pass

def log(*args, file= None, **kwargs): 
    try:
        if log_file == None: 
            log_file = open("logs/task_7/main_log", "a") 
            print(*args, **kwargs, file=log_file)
            log_file.close()
        else: 
            print(*args, **kwargs, file=log_file)
    except: 
        print("Unable to write to log")
        print(*args, **kwargs)

log("#"*26, "Starting new run of task 6", sep="\n")

# Convolutional neural networks

def make_many_convolutions_model_unnormalized(training_set, label_set, learning_rate = 0.004): 
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

def make_many_convolutions_model_batch_normalized(training_set, label_set, learning_rate = 0.004): 
    # n_labels = label_set.shape[1]
    num_classes = 10

    # Single later, but several convolutional layers
    output_model = Sequential()
    output_model.add(keras.layers.BatchNormalization()                                                                                           ) 
    output_model.add(Conv2D(32, kernel_size=(3, 3),activation='linear',input_shape=(28,28,1),padding='same')                                     ) 
    output_model.add(Activation("sigmoid")                                                                                                       ) 
    output_model.add(MaxPooling2D((2, 2),padding='same')) # I use max-polling because I decide to be stupid and not listen to others 
    output_model.add(keras.layers.BatchNormalization()                                                                                           ) 
    output_model.add(Conv2D(64, (3, 3), activation='linear',padding='same')                                                                      ) 
    output_model.add(Activation("sigmoid")                                                                                                       ) 
    output_model.add(MaxPooling2D(pool_size=(2, 2),padding='same')                                                                               ) 
    output_model.add(keras.layers.BatchNormalization()                                                                                           ) 
    output_model.add(Conv2D(128, (3, 3), activation='linear',padding='same')                                                                     ) 
    output_model.add(Activation("sigmoid")                                                                                                       )                   
    output_model.add(MaxPooling2D(pool_size=(2, 2),padding='same')                                                                               ) 
    output_model.add(Flatten()                                                                                                                   ) 
    output_model.add(keras.layers.BatchNormalization()                                                                                           ) 
    output_model.add(Dense(128, activation='linear')                                                                                             ) 
    output_model.add(Activation("sigmoid")                                                                                                       )                   
    output_model.add(Dense(num_classes, activation='softmax')                                                                                    ) 

    output_model.compile( 
                    loss= "categorical_crossentropy",
                    metrics=["acc"],
                    optimizer=keras.optimizers.Adam(learning_rate)
    )
    # output_model.summary()
    return output_model


def task_7_1(
    learning_x = x_number_train_raw[:,:,:,None],
    learning_y = y_number_train, 
    training_set_name = "MNIST"
): 

    my_n_epochs = GLOBAL_N_EPOCHS*2

    # Test the different models
    many_convolutions_model_unnormalized = make_many_convolutions_model_unnormalized(learning_x, learning_y)
   
    log("Start training the many convolutions net")
    many_convolutions_history_unnormalized = many_convolutions_model_unnormalized.fit(learning_x, learning_y, epochs = my_n_epochs, batch_size=DEBUG_BATCH_SIZE, validation_split=0.2)
    fig = plot_history(many_convolutions_history_unnormalized, "Task 7 "+training_set_name+" (Many convolutions (Unnormalised)) ")
    reckless_savefig(fig, "plots/task_7/"+training_set_name+"many_convolutions_network_unnormalized.png")


    many_convolutions_model_batch_normalized = make_many_convolutions_model_batch_normalized(learning_x, learning_y)
    
    many_convolutions_history_batch_normalized = many_convolutions_model_batch_normalized.fit(learning_x, learning_y, epochs = my_n_epochs, batch_size=DEBUG_BATCH_SIZE, validation_split=0.2)
    fig = plot_history(many_convolutions_history_batch_normalized, "Task 7 "+training_set_name+" (Many convolutions (normalised)) ")
    reckless_savefig(fig, "plots/task_7/"+training_set_name+"many_convolutions_network_batch_normalized.png")







if __name__ == "__main__":

    task_7_1(
        learning_x         = x_fashion_train_raw[:,:,:,None],
        learning_y         = y_fashion_train,
        training_set_name  ="Fashion_MNIST"
    )