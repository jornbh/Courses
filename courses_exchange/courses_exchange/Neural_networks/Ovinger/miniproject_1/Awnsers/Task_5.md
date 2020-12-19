# Task 5
>## Questions
>### Exercise 5: Going deeper: tricks and regularization (8 points)
>
>#### Description
>
>Adding hidden layers to a deep network does not necessarily lead to a straight-forward improvement of performance. Overfitting can be counteracted with regularization and dropout. Batch normalization is supposed to mainly speed up convergence. Since the MNIST dataset is almost perfectly solved already by a one-hidden-layer network we use the Fashion-MNIST dataset in this exercise.
>
>1. Add one or two hidden layers with 50 hidden neurons (each) and train the network for a sufficiently long time (at least 100 epochs). Since deep models are very expressive you will most probably encounter overfitting. Try to improve the best validation scores of the model (even if it is only a minor improvement) by experimenting with batch_normalization layers, dropout layers and l1- and l2-regularization on weights (kernels) and biases. (4 pts)
>2. After you have found good settings, plot the learning curves for both models, naive (=no tricks/regularization) and tuned (=tricks + regularized), preferably together in a comparison plot. Discuss your results; refer to the model performance with only 1 hidden layer. (2 sentences max.) (2pts)
>3. Fit your best performing (probably regularized deep) model also to MNIST for having a reference for the next exercise. Plot the resulting learning curves. (2 pts)
>
>#### Solution

## Awnsers 

1. All the results can be seen below, and all of them seem to be acting very strangely.ch The model seems to have a sudden drop in accuracy when using a learning rate of 0.7 if batch normalization or l2 normalization is used. I was not able to pinpoint why this hapened, since I expected numerical instability to cause some kind of oscilation, which would be ovserved as a gradual drop. This problem was solved by changing the learning rate to 0.1, but there might exist more reasonable ways to handle this, like an adaptive step-size with potential backsteping, or potentially changing the loss function. But this causes the model to not fully converge in the 100 epochs, but from the observations that can be seen: 

None of the changes seem to have any noticeable effect.
   1.  
   2. Adding l1 regularisation 
   3. Adding l2 regularisation 
   4. adding bias


1. Addding a bias term
   * Adding a bias term makes the network able to represetn more, as it can take in a constant 1 as one of its inputs to each neuron. This can make the network able to learn more, but in this case, there was no visible effect. 
   * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_5/Task_5_1_case%3A_%7B%7Chas_bias%7C%3A_True%7D_epochs%3A_100.png)
2. Adding l1 regularisation of 0.001 
   * Adding a l1 regularisation term 
   * Usually, if a network is overfited, it puts a very large emphasis on a specific path, so certain wiaghts can become very large or very small. We can add regularisation terms to try to combat this, since the general intuition about pictures is rather that arrge areas are usually more important than single pixels. In this case, however, the difference between modified and unmodified was not vert noticeable. 
   * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_5/Task_5_1_case%3A_%7B%7Cl1_regularisation%7C%3A_0.001%7D_epochs%3A_100.png) 
3. Adding a dropout of 30%
   * This is usually done in order to keep the network from relying too much on a single path when identifying something. In practice, this aparantly works very well. In this case, the difference was not very noticeable, since the network did not start overfiting yet. 
   * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_5/Task_5_1_case%3A_%7B%7Cdropout_rate%7C%3A_0.3%7D_epochs%3A_100.png          )
4. Adding l2-regularisation
   * Same idea as with l1-regularisation, except for that this one will punish single neurons who have extreme values harder than multiple neurons simply having high values. Onece again, the difference is not very noticeable. 
   * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_5/Task_5_1_case%3A_%7B%7Cl2_reguarisation%7C%3A_0.001%7D_epochs%3A_100.png)
5. Adding batch-normalisation. 
   * This is mosyly a trick to reduce the effects of the vanishing gradient problem, so it stands to no suprise that the difference is hardly noticeable here as well
* ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_5/Task_5_1_case%3A_%7B%7Cis_batch_normalized%7C%3A_True%7D_epochs%3A_101.png)

6. Ploting the best case against the unchanged example 
   * In this case, the l1-regularized network was the one who performed the best. But as we can see form the plot, there is almost no difference between the two. Potentially, if more running time were to be given, we might be able to see some difference as the networks would start to overfit, git in this case, we will continue to use the unmodifyed network for Exercise 6, due to simplicitty. 
   * ![](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_5/Task_5_2_no_movification_vs_%7B'l1_regularisation'%3A%200.001%7D_(Best_modification).png)
