# Task 2

>## Questions:
>
>### Exercise 2: No hidden layer (10 points)
>
>#### Description
>
>Define and fit a model without a hidden layer (since we will use multi-layer models later in this project, you can define a general constructor function for models with an arbitrary number of hidden layers already at this point). (1 pt for each step)
>
>1. Use the softmax activation for the output layer.
>2. Use the categorical_crossentropy loss.
>3. Add the accuracy metric to the metrics.
>4. Choose stochastic gradient descent for the optimizer.
>5. Choose a minibatch size of 128.
>6. Fit for as many epochs as needed to see no further decrease in the validation loss.
>7. Plot the output of the fitting procedure (a history object) using the function plot_history defined above.
>8. Determine the indices of all test images that are misclassified by the fitted model and plot some of them using the function `plot_some_samples(x_test, y_test, yhat_test, error_indices)`. Explain the green and red digits at the bottom of each image.
>9. Repeat the above steps for fitting the network to the Fashion-MNIST dataset.
>
>Hints:
>
>- Read the keras docs, in particular [Getting started with the Keras Sequential model](https://keras.io/getting-started/sequential-model-guide/).
>- Have a look at the keras [examples](https://github.com/keras-team/keras/tree/master/examples), e.g. [mnist_mlp](https://github.com/keras-team/keras/blob/master/examples/mnist_mlp.py).

## Awnsers

1. (Just implementation)
2. (Just implementation)
3. (Just implementation)
4. (Just implementation)
5. (Just implementation)
6. We end up using 50 epochs here, since it is consistent with later exercises, making debugging easier. But going above 50 might also be usefull, as it seems like the model still has not converged completely. Still, it seems like most of it has converged.

7. The history of the two objects:
    - ![Plot of learing ](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_2/Task_2_number.png)Task_2_fashion.png)
8. The red number corresponds to the incorrect estimate, while the green one represents the ground truth.
    - ![Pic](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_2/Task_2_Some_mislabeled_number_samples_after_training.png)

9. The plots and code for fashion MNIST have already been added. (But the first picture, for numbers got the wrong name)
    - ![Plot of learing ](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_2/Task_2_fashion.png)
    - ![Pic](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_2/Task_2_Some_mislabeled_fashion_samples_after_training.png)
