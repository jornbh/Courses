# Task 1

## Questions

> ### Exercise 1: Data import and visualization (4 points)
> 
> #### Description
> 
> #### Loading the data
>
> The datasets we use in this project (MNIST, Fashion-MNIST) consists of grayscale images with 28x28 pixels. Keras comes with a convenient in-built [data importer](https://keras.io/datasets/) for common datasets.
>
>1. As a warm-up exercise, use this importer to (down-)load the MNIST and Fashion-MNIST dataset. Assign useful variables to test & train images and labels for both datasets respectively. (2 pts)
>2. Use the corresponding plotting function defined above to plot some samples of the two datasets. What do the green digits at the bottom left of each image indicate? (1 sentence max.) (2 pts)
>
>The low resolution (and grayscale) of the images certainly misses some information that could be helpful for classifying the images. However, since the data has lower dimensionality due to the low resolution, the fitting procedures converge faster. This is an advantage in situations like here (or generally when prototyping), were we want to try many different things without having to wait too long for computations to finish.

## Awnsers

1. (No question asked, but a plot was given)
2. ![Display of the samples](https://raw.githubusercontent.com/jornbh/courses_exchange/master/Neural_networks/Ovinger/miniproject_1/Awnsers/Plots_and_pictures/Exercise_1/some_samples.png)
3. The green numbers below each number is theri index in the table representing each different class ( This list is 1 -indexed )
