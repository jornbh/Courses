# Task 4

>## Questions
>
>### Exercise 4: Model performance as a function of number of hidden neurons (8 points)
>
>#### Description
>
>Investigate how the best validation loss and accuracy depends on the number of hidden neurons in a single layer.
>
>1. Fit a reasonable number of models (e.g. 5) with different hidden layer sizes (between 10 and 1000 hidden neurons) to the MNIST dataset. You may use the Adam optimizer and a meaningful number of epochs (overfitting!). (3 pts)
>2. Plot the best validation loss and accuracy versus the number of hidden neurons. Is the observed trend in accordance with the [general approximation theorem](https://en.wikipedia.org/wiki/Universal_approximation_theorem)? If not, what might be practical reasons for the deviation? (2 sentences max.) (3 pts)
>3. Repeat steps 1. & 2. for the Fashion-MNIST dataset. (2 pts)
>
>In this exercise we fit each model only for one initialization and random seed. In practice one would collect some statistics (e.g. 25-, 50-, 75-percentiles) for each layer size by fitting each model several times with different initializations and the random seeds. You may also want to do this here. It is a good exercise, but not mandatory as it takes quite a bit of computation time.
>
>#### Solution

## Awnsers

1. (Implementation)
2. I messed in the original implementation, and the number of neurons in the hidden layer was writtn as depth 
   1. Best validation loss for each layer:
   2. Best accuracy for each layer
      * ![smallest fashion validation loss](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_4/smallest_losses_fashion.png)
      * ![Smallest number validation loss](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_4/smallest_losses_number.png)
      * It is not possible to see exactly how the validation loss is affected by the number of neurons in the single hiden layer. But it seems like adding a lot of hidden neuons causes the validation loss to go up. Most likely, this is because of the fact that the width increases the expressive power of the neural network, without increasing the bias towards learning the patterns that are common for all the pictures, but this is most likely wrong, since the plots form the 10 and 752 neuron case do not seem to entyrely agree with this
         * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_4/Redundant_plots/Exercise_4_10_hidden_neurons.png)
         * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_4/Redundant_plots/Exercise_4_752_hidden_neurons.png) 
      * It might be a bit more reasonable to simply state that for 50 epochs, the network does not imporove significantly by adding more hidden neurons, since most of the differences seem to come from the fact that the validation loss is so noisy. 
      * This seems to fit well with when we retry for the number MNIST 
      * ![Retry number MNIST](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_4/retry_smallest_losses_number.png)

3. The fashion example is rather similair to the number example, and seem to display the same pattern as the number model.