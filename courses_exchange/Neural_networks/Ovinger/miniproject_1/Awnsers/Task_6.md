# Task 6

>## Questions
>### Exercise 6: Convolutional neural networks (CNNs) (10 points)
>
>#### Description
>
>Convolutional neural networks have an inductive bias that is well adapted to image classification.
>
>1. Design a convolutional neural network, play with different architectures and parameters. Hint: You may get valuable inspiration from the keras [examples](https://github.com/keras-team/keras/tree/master/examples). (4 pts)
>2. Plot the learning curves of the convolutional neural network for MNIST and Fashion-MNIST. (4 pts)
>3. How does the CNN performance compare to the so far best performing (deep) neural network model for the two data sets? (2 sentences max.) (2 pts)
>
>#### Solution


## Awnsers

1. (Implementation) 
   1. When I implemented this, I used data from the previous exercise that seemed to indicate that none of he made any change on 
   2. I only tried two cases for the convolutional network. One that had only a single hidden layer, but with three convolutional layers, and one with a single, larger convolutional layer, but with two fully connected layers. In both cases, I ended up using max-pooling, even though we were told not to. It seemed easier, and makes intuitively more sense when looking for features, (Alhough, i guess when going for intuition, and ignoring the math, taking the max based on aboslute values makes even more sense when we are looking for certain properties.)
2. I somehow managed to crash the program before it was able to train the network with fewer convolutions. But I still have the plots for the one with many convolutions: 
   * ![Many convolutions on Fashion MNIST](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_6/Fashion_MNISTmany_convolutions_network.png)
   * ![Many convolutions on Number MNIST](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_6/MNIST_many_convolutions_network.png)
3. The Convolutional neural networks are more or less able to blow the old networks completely out of the water. We do see some difference in the accuracy between the training and the test-examples when it comes to fashion-MNIST, but the old networks were hardly able to get an accuracy above 85% on the training set. In the case of teh normal number MNIST, it has near perfect accuracy in both the training cases, as well as in the validation case. 
   * ![Nonconvolving network](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_6/task_6_naive_nonconvolving_depth=2_epochs_100.png)