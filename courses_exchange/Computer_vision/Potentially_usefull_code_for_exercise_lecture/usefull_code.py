import cv2
import matplotlib.pyplot as plt
import numpy as np
#TODO Make some usefull code

# No need to do a sobel first. 
# Canny edge detection
img = cv2.imread('opencv-logo2.png',0)
edges = cv2.Canny(img,100,200)
plt.imshow(edges, cmap="gray")
plt.savefig("filler.jpg")
plt.close()


#Moments (Not sure if this is the right one, though)

img = cv2.imread('opencv-logo2.png',0)
ret,thresh = cv2.threshold(img,127,255,0)
contours,hierarchy = cv2.findContours(thresh, 1, 2)

cnt = contours[0]
M = cv2.moments(cnt)

# M_pq : 
for i in M:
    print(i, ": ", M[i])



# Hough circle transform 

import cv2
import numpy as np

img = cv2.imread('opencv-logo2.png',0)
img = cv2.medianBlur(img,5)

cimg = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

circles = cv2.HoughCircles(img,cv2.HOUGH_GRADIENT,1,20,
                            param1=50,param2=30,minRadius=0,maxRadius=0)

circles = np.uint16(np.around(circles))
for i in circles[0,:]:
    # draw the outer circle
    cv2.circle(cimg,(i[0],i[1]),i[2],(0,255,0),2)
    # draw the center of the circle
    cv2.circle(cimg,(i[0],i[1]),2,(0,0,255),3)

cv2.imshow('detected circles',cimg)
cv2.waitKey(0)
cv2.destroyAllWindows()