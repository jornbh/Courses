# Task 7

>## Questions
>### Exercise 7: Sigmoidal activation function and batch-normalization (6 >points)
>
>#### Description:
>
>In the original publication of batch normalization [Ioffe and Szegedy, 2014](https://arxiv.org/pdf/1502.03167.pdf), the authors mention a particularly beneficial effect of their method on networks with sigmoidal activation functions. This is because such networks usually suffer from saturating activations/vanishing gradients. Here we want to reproduce this behaviour (Chose either MNIST or Fashion-MNIST for this exercise).
>
>1. Implement the same convolutional network as in the previous exercise, but using the sigmoid activation function instead of the standard choice ReLU. Train the network for a reasonable amount of time. What do you observe? (1 sentence max.) (3 pts)
>2. Add batch-normalization layers to all convolutional and fully-connected layers (i.e. before each layer with learnable parameters). How does the performance change? Can the network reach the ReLU-CNN performance of the previous exercise? (1 sentence max.) (3 pts)
>3. **BONUS (optional, not graded**): Investigate our initial guess that saturating activity/vanishing gradients might be the cause of this behaviour. For that, create histograms of the hidden activitions for different hidden layers for the sigmoid-CNN and the sigmoid-CNN with batch-normalization (counting over both, samples and neurons per layer). You may only chose layers with learnable parameters. What do you observe?
>Hint: You can use the [keract](https://github.com/philipperemy/keract) package to access neural activation values for all layers of your network model.
>
>
>
>#### Solution:

## Awnsers

1. We see that for the training example it is mostly similair to the provious cases, but when it comes to the validation test, it suffers greatly from some kind of noise or uncertainty. 
   * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_7/Fashion_MNISTmany_convolutions_network_unnormalized.png)
   * The result of the sigmoidal function is that the accuracy of the validation seems to have remained the same. In this case, the network is not very deep, so we do not suffer from the vanishing gradients problem, so we do not take a severe hit in the number of itterations needed for convergence eiter. (Both the case form Exercise 5 and 6 seems to have more or less converged after 50 epochs)
   * In the case of the batch-normalized example. It can be seen that it reaches a similair level of accuracy at the validation set. 
   * ![Picture not found](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_7/Fashion_MNISTmany_convolutions_network_batch_normalized.png)
   * Since the network is not particulary deep, the result of the batch normalisation does not end up leadning to a large improvement in convergence rate, since both validation sets seems to convergge somewhere around epoch 20, and the training sets seem to have almost completely converged at epoch 75. Still, if this had been a deep network, the difference would have been more noticeable, and the batch-normalized network should have conberged faster.