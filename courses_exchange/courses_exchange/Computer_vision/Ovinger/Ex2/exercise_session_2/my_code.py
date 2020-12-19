#%% [markdown]
# # Exercise Session 2: Convolution, Filtering and Gradients
#%% [markdown]
# The goal of this exercise is to:
# * Understand and apply a convolutional filter to an image
# * Compare the computational complexity of separable and non-separable filters
# * Compute image gradients and apply them to real-world images

#%%
get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt
import cv2
import time
import scipy.signal as conv

plt.rcParams['figure.figsize'] = (15, 15)
plt.rcParams['image.cmap'] = 'gray'

#%% [markdown]
# ## Exercise 1: Applying Convolutional Filters
#%% [markdown]
# Assume we are given a gray-scale image $I[x, y]$, of size $W \times H$, such that $1 \leq x \leq W$,
# and $1 \leq y \leq H$. We want to apply a filter $F[i, j]$ to image $I$. The filter $F$ is of size $(2N + 1) \times (2M + 1)$, such that $−N \leq i \leq N$, and $−M \leq j \leq M$.
# 
# The result can be computed as
#%% [markdown]
# \begin{align}
# R[x, y] = (I ∗ F)[x, y] = \sum_{i=-N}^{N} \sum_{j=-M}^{M} I[x − i, y − j]~F[i, j]
# \end{align}
# 
#%% [markdown]
# * Implement a function ```R = applyImageFilter(I,F)``` that takes a
# gray-scale image $I$ and a filter $F$ as inputs, and returns the result of the convolution
# of the two.
# Note: There are many python libraries providing convolution function to convolve an image with a filter, 
# but in this exercise you are requested to implement your own. This will help your understanding
# of how convolutional filters work.
#     -  To avoid numerical issues, make sure $I$ and $F$ are of type float. To understand
# why, think what would happen if you add two unsigned 8-bit numbers when
# computing the convolution, for example 240 and 80.
#     -  What happens when computing $R[x, y]$ near the border of the image? What
# would you propose to deal with this issue?  

#%%
# Insert code only at the places indicated with 'CODE HERE'
def applyImageFilter(I, F):
    # First input parameter: I
    #     Input image. It should be a 2D matrix. According to the notation in the description, it has W rows and 
    #     H columns.
    # Second input parameter: F
    #     Filter used for the convolution. It should be a 2D matrix. According to the notation in the decription,
    #     it has (2N+1) rows and (2M+1) columns.

    # If image is not of type float, convert it to float
    if not np.issubdtype(I.dtype,float):
        I = np.float64(I)
        
    # If the filter is not of type float, convert it to float
    if not np.issubdtype(F.dtype,float):
        F = np.float64(F)
    
    # Shape of Filter
    N_, M_ = F.shape
    
    # Check whether the dimensions of input are accurate, they should be odd
    if not N_%2==1: 
        print(I.shape)
        raise ValueError('Number of rows in the filter must be odd')
        
    if not M_%2==1:
        raise ValueError('Number of columns in the filter must be odd')
    
    # Compute the values for N,M which is used in the above description. 
    N = np.int64((N_-1)/2)
    M = np.int64((M_-1)/2)
    
    
    # Shape of the input image
    W, H = I.shape

    # ******************************************
    # Initialize output matrix R
    # ******************************************
    # CODE HERE
    # R = 
    # after implemntation remove the line below
    raise ValueError('Not implemented yet.') 
    # ******************************************
    
    
    # Output image size
    W_R, H_R = R.shape
    

    # ******************************************
    # Perfrom the convolotion here
    # Hint 1: You can implement it with 4 nested for loops
    # Hint 2: Outer two for loops should iterate over the pixels in output image
    # Hint 3: Inner two for loops are used to compute the R[x,y] value
    # ******************************************
    # CODE HERE 
    # after implemntation remove the line below
    raise ValueError('Not implemented yet.') 
    # ******************************************
                    
    return R

# Below code is to verify your implementation of the convolution
# we compare your output with the scipy.signal implementation of the convolution
# and the error should be zero

# Read the sample image
img = cv2.imread('images/res1.png',0) 

# Generate a random filter F of size 3 x 3
F = np.random.randn(5,3) 

# Perform convolution with the function you implemented
output_1 = applyImageFilter(img,F)

# Perform convolution with scipy.signal implementation of convolution
# NOTE: specify the mode based on the version of convolution you implemented
# It's either 'same' or 'valid'
output_2 = conv.convolve2d(img, F , mode='same')

# Error
print('Input shape: ' + str(img.shape))
print('Output shape: ' + str(output_1.shape))
print('Error: ' + str(np.sum(np.abs(output_1 - output_2))))
# if the implmentation is accurate, the error will be 0

#%% [markdown]
# ## Exercise 2: Image Smoothing
#%% [markdown]
# Convolutional filters have many uses. A very common one is to smooth or soften an
# image. A typical smoothing filter is the Gaussian Filter, which follows the equation of a
# 2D Gaussian probability distribution. It is an important preprocessing step typically to reduce the amount of noise.  When constructing a Gaussian filter we can specify the size and standard deviation (σ) of the underlying Gaussian function.
#%% [markdown]
# * To create a Gaussian filter use the function given below. 

#%%
def gaussian_filter(fSize, fSigma):
    x, y = np.mgrid[-fSize//2 + 1:fSize//2 + 1, -fSize//2 + 1:fSize//2 + 1]
    g = np.exp(-((x**2 + y**2)/(2.0*fSigma**2)))
    return g/g.sum()

#%% [markdown]
# * What are fSize and fSigma?
# * How do fSize and fSigma affect the filter’s shape and size? 
# * Visualize different filters by giving different values to these parameters.
# * If you are given fSigma, how would you choose fSize?

#%%


#%% [markdown]
# * Apply Gaussian filtering to 'res1.png'
# * To apply the filter to an image use cv2.GaussianBlur(img, fSize, fSigma)
# * Experiment with different values of fSigma. How does the amount of smoothing vary with this parameter?

#%%


#%% [markdown]
# As you saw in the first exercise of this session, applying the filter near the border of
# the image is not a well-defined problem. It is possible to deal with this issue through
# different approaches. Luckily, cv2.GaussianBlur implements different ways of dealing with border effects.
# Here you will try them out and understand what each of them does.
#%% [markdown]
# *  Apply a Gaussian filter with fSigma = 10 and fSize = 25 to 'res1.png'
# * Use borderType flag of cv2.GaussianBlur function to define what kind of border should be added around the image (cv2.BORDER_CONSTANT, cv2.BORDER_REFLECT, cv2.BORDER_REPLICATE).
# 

#%%


#%% [markdown]
# ## Exercise 3: Edge detector
#%% [markdown]
# Edge detection is one of the most important operations in Computer Vision. In this exercise we will investigate how it
# can be performed and what information can be obtained from it.
# 
# One of the ways to detect edges is to compute image intensity gradients. They can be approximated by convolving image with Sobel filters. They consist of 2 $3 \times 3$ filters:
# 
# \begin{equation*}
# S_x = \frac{1}{8}
# \begin{bmatrix}
# -1 & 0 & +1\\
# -2 & 0 & +2\\
# -1 & 0 & +1\\
# \end{bmatrix}
# \hspace{2cm}
# S_y = \frac{1}{8}
# \begin{bmatrix}
# -1 & -2 & -1\\
# 0 & 0 & 0\\
# +1 & +2 & +1\\
# \end{bmatrix}
# \end{equation*}
# 
# where $S_x$ computes the partial derivative of the image in the horizontal direction, while $S_y$ does it in the vertical direction.
# 
# 
#%% [markdown]
# * Compute the image derivatives in x- and y-directions using Sobel filters. You can use for that function ``` cv2.Sobel()```

#%%
img_coins = cv2.imread('images/coins.png',0)
#Compute gradient in x-direction
grad_x = np.zeros(coins_img.shape)
#Compute gradient in y-direction
grad_y = np.zeros(coins_img.shape)

#%% [markdown]
# * Visualize the image gradients. How can you explain the differences between the two results? 

#%%
fig = plt.figure()
plt.subplot(1,2,1)
grady = plt.imshow(grad_x,cmap="jet")
plt.title("Gradient x")
plt.colorbar(fraction=0.046, pad=0.04)
plt.subplot(1,2,2)
gradx = plt.imshow(grad_y,cmap="jet")
plt.title("Gradient y")
plt.colorbar(fraction=0.046, pad=0.04)

#%% [markdown]
# Mathematically, the goal of the filters $S_x$ and $S_y$ is to approximate the derivatives of the image with respect to 
# the forizontal and vertical directions respectively, such that
# \begin{equation*}
# \nabla I_x(x,y) = (I*S_x)[x,y]
# \quad\mathrm{and}\quad
# \nabla I_y(x,y) = (I*S_y)[x,y]
# \end{equation*}
# 
# Therefore, the gradient of an image at each point is a 2D vector
# \begin{equation*}
# \nabla I =
# \begin{bmatrix}
# \nabla I_x\\
# \nabla I_y
# \end{bmatrix}
# \end{equation*}
# 
# This vector can be computed for every pixel. Its magnitude and phase can be computed as
# \begin{equation*}
# || \nabla I || = \sqrt{(\nabla I_x)^2 + (\nabla I_y)^2} \\
# \angle \nabla I = atan2(\nabla I_x, \nabla I_y)
# \end{equation*}
# 
# $\it{Note:}$ we use atan2() instead of atan() to be able to determine the right quadrant of the phase.
#%% [markdown]
# * Using the previously computed image gradients and the above formula compute the gradient magnitude and gradient phase.
# 

#%%
#Compute gradient magnitude
grad_mag = np.zeros(coins_img.shape)
#Compute gradient phase
grad_phase = np.zeros(coins_img.shape)

#%% [markdown]
# * Visualize the gradient magnitude and phase images along with the original image. Where does the gradient have high 
# magnitude? How does the phase change along the coin border?

#%%
fig = plt.figure()
plt.subplot(1,3,1)
plt.imshow(img_coins)
plt.subplot(1,3,2)
grady = plt.imshow(grad_mag,cmap="jet")
plt.title("Gradient magnitude")
plt.colorbar(fraction=0.046, pad=0.04)
plt.subplot(1,3,3)
gradx = plt.imshow(grad_phase)
plt.title("Gradient phase")
plt.colorbar(fraction=0.046, pad=0.04)

#%% [markdown]
# ## Exercise 4: Seperable Filters
#%% [markdown]
# 
# As seen in class, certain types of 2D filters can be thought of as the composition of two
# 1-dimensional filters. These are called Separable Filters, and can be computed more
# efficiently than those who are non-separable.
# 
# For example, the Sobel filter $S_x$ can be decomposed as
#  
# \begin{equation*} 
# \begin{bmatrix}
# -1 & 0 & +1\\
# -2 & 0 & +2\\
# -1 & 0 & +1\\
# \end{bmatrix} = 
# \begin{bmatrix}
# +1 \\
# +2 \\
# +1 \\
# \end{bmatrix}
# \begin{bmatrix}
# -1 & 0 & +1
# \end{bmatrix}
# \end{equation*} 
# 
# which means that the convolution of I with the 2D filter $S_x$ can be simplified to two
# convolutions with 1D filters,
# 
# \begin{equation*}  
# I * S_x = 
# \frac{1}{8} I *
# \begin{bmatrix}
# -1 & 0 & +1\\
# -2 & 0 & +2\\
# -1 & 0 & +1\\
# \end{bmatrix}  
# \end{equation*} 
# 
# \begin{equation*} 
# = 
# \frac{1}{8}\Bigg(I *
# \begin{bmatrix}
# +1\\
# +2\\
# +1\\
# \end{bmatrix} \Bigg) *
# \begin{bmatrix}
# -1 & 0 & +1
# \end{bmatrix} 
# \end{equation*} 
# 
# For the comparison of the computational complexity of separable and non-separable
# filters, check out the lecture notes.
# 
# * We will compare the performance of separable and non-separable filters.
#     -  Create a separable filter (for example a Gaussian filter, you can use  ```gaussian_filter``` function given above) 
#     -  Use ```decomposeSeperableFilter``` to decompose the seperable filter to two, 1D filters. 
#     -  Implement ```applyImageSepFilter``` function. You can use ```applyImageFilter``` function implemented in Applying Convolutional Filters (Hint: two lines of code)
#     -  Performe convolution on ```img``` matrix and measure the time for ```decomposeSeperableFilter``` and ```applyImageSepFilter```. 
#     -  Repeat the test with several increasing filter sizes.
#     -  measure the elapsed time for each filtering. To get the start and end time, use time.time()
#     -  Plot the results in a graph (elapsed time) vs (filter size).
#     -  What is your conclusion ? Is filtering with separable filters always faster than with non-separable ones?

#%%
# Insert code only at the places indicated with 'CODE HERE'
def decomposeSeperableFilter(F): 
    # DO NOT CHANGE THIS FUNCTION
    h = [1]
    for i in range(1,s):
            h.append(np.sum(F[:,i])/(np.sum(F[:,0])))
    h = np.asmatrix(np.array(h))
    v = np.asmatrix(F[:,0]).transpose()
    return v, h

def applyImageSepFilter(I, F_v, F_h):
    # ******************************************
    # Implement the convolution with the two 1D filters and assign the result to 'output' variable
    # Hint: you only need two lines of code
    # ******************************************
    # CODE HERE 
    # after implemntation remove the line below
    raise ValueError('Not implemented yet.') 
    # ******************************************
    
    return output
           
# Sample image
img = cv2.imread('images/res1.png',0)
img = img[150:200,150:200] # We crop the image, becasue larger images take considerably longer time to 
                           # perform convolution with large filters
    
# Filter sizes to try out. Since we consider filter with odd heigh and width, we start from 3 and incrment it by 2 till 
# 20. 
filter_sizes = range(3,30,2)

# ******************************************
# Perfrom the comparison here
# Hint: Iterate through the filter_sizes and in each iteration perform convolution with the 2D and 1D filters
#         using applyImageFilter and applyImageSepFilter functions respetively. Measure the time for each execution. 
#         Store the execution times and plot them at the end 
# Note: Performing convolutions with larger filters could take longer time (sometimes more than a minute)
# ******************************************
# CODE HERE 
# after implemntation remove the line below
raise ValueError('Not implemented yet.') 
# ******************************************


