# Task 3

>## Qusestions
>
>### Exercise 3: One hidden layer, different optizimizers & overfitting (10 points)
>
>#### Description
>
>Train a network with one hidden layer and compare different optimizers.
>
>1. Use one hidden layer with 128 units and the 'relu' activation. Use the [summary method](https://keras.io/models/>about-keras-models/) to display your model in a compact way. (1 pt)
>2. Fit the model for 50 epochs with different learning rates of stochastic gradient descent (SGD). (1pt)
>3. Replace the stochastic gradient descent optimizer with the [Adam optimizer](https://keras.io/optimizers/#adam). (1pt)
>4. Plot the learning curves of SGD with a reasonable learning rate (i.e. in the range [0.01,0.1]) together with the learning curves of Adam in the same figure. Take care of a reasonable labeling of the curves in the plot. (2pts)
>5. Answer the questions below. (4pts)
>6. Run the network (using the Adam optimizer) on the Fashion-MNIST dataset and plot the learning curves using the plot_history function defined above. (1pt)



> ## Theoretical questions:

# Task 3 awnsers

> **Question**: What happens if the learning rate of SGD is A) very large B) very small? Please answer A) and B) with one full sentence each (double click this markdown cell to edit).

**Answer**:

A) The range 0.01 to o.1, the stocastic gradiet descent converges more slowly when the learning rate is low. As the learning-rate increases, the accuracy of the training set increases more quickly. We also start seeing that the distance between the trainging set and the verification set becomes more noticeable as the learning rate increases.

B) The Adams method has the advantage when the learning rate is very low, but it ends up performing more poorly when learnign rate is around 0.02. In addition, in these small examples, the asymptotic value of the adams method also seems to be noticeably worse tha

> **Question**: At which epoch (approximately) does the Adam optimizer start to overfit (on MNIST)? Please answer with one full sentence.

**Answer**: From the pictures, it can be seen that the adams method starts overfiting roughly around epoch 4

> **Question**: Explain the qualitative difference between the loss curves and the accuracy curves with respect to signs of overfitting. Please answer with at most 3 full sentences.


**Answer**: The accuracy function is based on slimple true or false for weterer or not the value that was guessed was correct or not. Loss, on the other hand is also affected by how confidently the model states something, giving a lower loss if the model is very confident on all the correct statements, but also taking a seere hit if any of the estimates it was so confident in proves to be wrong. So, if the loss decreases significantly, without any significant change in the accuracty, it is most likely a sign of that the model is overfiting. 


## Awnsers

 1. (Just implementation)
 2. (Just implementation)
 3. (Just implementation)
 4. All the plots: 
    1. <!-- TODO add plots -->


![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_2/Task_2_fashion.png)


![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.01,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.02,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.03,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.04,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.05,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.06,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.07,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_fashion,_learning_rate:_0.08,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.01,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.02,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.03,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.04,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.05,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.06,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.07,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.08,_epochs:_50.png)
![Picture not found](https://github.com/jornbh/courses_exchange/tree/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_3/Task_3_learning_set:_number,_learning_rate:_0.09,_epochs:_50.png)




 1. See above
 2. Add plots from actually running on the fashion set
<!-- ![Picture not found](./Plots_and_pictures/Exercise_3/Task 3 learning_set: number, learning_rate: 0.04, epochs: 50.png) -->







