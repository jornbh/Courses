
#%%
import cv2 
import numpy as np 
import matplotlib.pyplot as plt
import cmath






def RGB_2_HSV( RGB_image ): 
    complex_image= np.full(RGB_image.shape[0:2], 0j)
    value = np.mean(RGB_image, axis=2)
    
    # Add all the numbers as complex numbers
    phase_difference = cmath.e**(cmath.pi * 1j / 3)
    for i in range(3): 
        complex_image += (1j * phase_difference**i) * (RGB_image[:,:,i] - value)


    saturation = np.abs( complex_image )
    
    real_part = np.real(complex_image) / saturation
    imaginary_part = np.imag(complex_image) / saturation
    hue = np.arctan2( imaginary_part, real_part)
    hue[np.isnan(hue)] = 0 # Quickfix of the hue
    height,width = RGB_image.shape[0:2]
    output = np.zeros( (height, width, 3) )
    output[:,:,0] = hue
    output[:,:,1] = saturation
    output[:,:,2] = value


    return output

def HSV_2_RGB(HSV_image):
    return None



#%% 
if __name__ == "__main__":
    image = cv2.imread("./apples.jpg", cv2.IMREAD_COLOR) # RGB as (dimensions), colours seem to be in the wrong order, though
    monotone = np.full( (10,10, 3), 100 )
    monotone = np.uint8(monotone)
    HSV_image = RGB_2_HSV(image)


    HSV_image = np.uint8(HSV_image)
    # display_image = image
    display_image = HSV_image
    print(display_image[:,:, 0])
    print(display_image[:,:, 1])
    print(display_image[:,:, 2])
    print(type( display_image[0,0,0] ))
    display_image[0,0,2] = 0
    plt.subplot(3,1,1)
    plt.imshow(display_image[:,:,0], cmap = "gray")
    plt.subplot(3,1,2)
    plt.imshow(display_image[:,:,1], cmap = "gray")
    plt.subplot(3,1,3)
    plt.imshow(display_image[:,:,2], cmap = "gray", interpolation= "nearest")

    # print(image.shape)
    plt.show()