# This will run task 1 if it is tun on a local computer 
# In a notebook, this should not be an issue
try:     
    from initialize_miniproject_1 import *
except: 
    print("Unable to import initialize_miniproject_1")
    pass
#TODO DELET THIS    
GLOBAL_N_EPOCHS = 300




def main():
    my_n_epochs = GLOBAL_N_EPOCHS*2
    # TODO Return this to normal 
    # test_case_list = [
    #             {"is_batch_normalized"  :     True      },
    #             {"l1_regularisation"    :     0.001     },
    #             {"l2_reguarisation"     :     0.001     },
    #             {"has_bias"             :     True      },
    #             {"dropout_rate"         :     0.3       },
    # ]
    
    test_case_list = [
                {"is_batch_normalized"  :     True      },

                {"l1_regularisation"    :     0.0003    },
                {"l1_regularisation"    :     0.001     },
                {"l1_regularisation"    :     0.005     },
                {"l1_regularisation"    :     0.02      },

                {"l2_reguarisation"     :     0.0003    },
                {"l2_reguarisation"     :     0.001     },
                {"l2_reguarisation"     :     0.005     },
                {"l2_reguarisation"     :     0.02      },
                {"dropout_rate"         :     0.3       },
                {"dropout_rate"         :     0.5       },
                {"dropout_rate"         :     0.7       },

    ]
    #TODO DELET THIS
    # Adding bias seemed to help, so we try it on a 300 set
    for ind,_ in enumerate(test_case_list):
        test_case_list[ind]["has_bias"] = True 
        
    # Part 1) 
    best_test_case   = None
    best_history     = None 
    highest_accuracy = 0 
    learning_rate = 0.0001
    for test_case in test_case_list:
        model = make_model(x_fashion_train, y_fashion_train, **test_case, learning_rate=learning_rate)
        model.summary()
        history = model.fit(    x_fashion_train,
                                 y_fashion_train,
                                 epochs = my_n_epochs,
                                 batch_size = DEBUG_BATCH_SIZE,
                                 validation_split = 0.2, 
                                 shuffle=True
                            )
        
        # Plot the result
        title = "Task_5_1_case:_{}_epochs:_{}".format(test_case, my_n_epochs)
        title = title.replace(" ", "_").replace("'", "|")
        path ="./plots/task_5/"+title+".png"
        fig = plot_history(history, title)
        reckless_savefig(fig, path)

        if max( history.history["acc"] ) > highest_accuracy:
            highest_accuracy = max( history.history["acc"] )
            best_history = history
            best_test_case = test_case

    # Part 2) 
    untouched_model = make_model(x_fashion_train, y_fashion_train, learning_rate=learning_rate)
    untouched_history = untouched_model.fit(x_fashion_train, y_fashion_train, epochs = my_n_epochs, batch_size = DEBUG_BATCH_SIZE, validation_split = 0.2)
    fig = comparison_plot( best_history, untouched_history, str(best_test_case), "Untouched model", "Task 5 best modification vs no modification ")
    path = "./plots/task_5/Task_5_2_no_movification_vs_{}_(Best_modification).png".format(best_test_case)
    reckless_savefig(fig, path)
    # Part 3) 
    best_trick_number_model = make_model(x_number_train,y_number_train, **best_test_case)
    number_history = history = model.fit(x_number_train, y_number_train, epochs = my_n_epochs, batch_size = DEBUG_BATCH_SIZE, validation_split = 0.2)
    title = "Task_5_3_best_case_{}_on_numbers_(epochs:_{})".format(best_test_case, my_n_epochs)
    fig = plot_history(number_history, title)
    reckless_savefig(fig, "./plots/task_5/"+title+".png")





# Implementation
##################################################


def make_hidden_layer_list(   
                        is_batch_normalized=False,
                        l1_regularisation=0,
                        l2_regularisation=0, 
                        has_bias=False, 
                        dropout_rate=0  
                    ):
    
    # Condition for if a layer should be included, and the layer
    conditional_layers= [
        (               True        ,   Dense(50, activation="linear", use_bias=has_bias)       ),
        (is_batch_normalized == True,   keras.layers.normalization.BatchNormalization()         ), 
        (l1_regularisation    >  0  ,   keras.layers.regularizers.l1(l1_regularisation)         ),
        (l2_regularisation    >  0  ,   keras.layers.regularizers.l2(l2_regularisation)         ),
        (dropout_rate         >  0  ,   keras.layers.Dropout(dropout_rate)                      ),
    ]

    # Filter the list
    hidden_layer_list = []
    for condition, layer in conditional_layers:
        if condition == True: 
            hidden_layer_list.append(layer)
    
    return hidden_layer_list




def make_model(     
                    features, 
                    labels, 
                    learning_rate =0.01 ,
                    is_batch_normalized=False,
                    l1_regularisation=0,
                    l2_reguarisation=0,
                    has_bias=False,
                    dropout_rate =0,
                ):

    
    Network = [
                        # Input layer
                            Dense(32, input_shape=features[0].shape, batch_size = DEBUG_BATCH_SIZE)   ,
                            Activation("linear")                                                      ,
    
                        #Hidden layers
                            *make_hidden_layer_list(), #! MAKE SURE YOU UNDERSTAND THE * on lists and tuples
                            *make_hidden_layer_list(), # (It is as if you were to write all the elements in cleartext, separated by commas)
                        
                        # Output layer
                            Dense( len(labels[0]) )                                                   ,
                            Activation("softmax")                                                     ,
            
            ]

    model = Sequential()

    for i in Network:
        print(i)
        model.add(i)


    model.compile(
                loss = "categorical_crossentropy", 
                metrics = ["acc"],
                optimizer= optimizers.Adam(learning_rate),
    )
    return model



if __name__ == "__main__":
    main()
