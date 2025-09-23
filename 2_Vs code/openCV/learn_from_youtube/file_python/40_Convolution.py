# Convolution 
import cv2
import numpy as np
import matplotlib.pyplot as plt

# Load image and convert to grayscale
img = cv2.imread("image/women_noise.png")
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

convo1 = cv2.filter2D(gray_img, -1, np.ones((3,3), np.float32)/9)
convo2 = cv2.filter2D(gray_img, -1, np.ones((5,5), np.float32)/25)

title = ['Gray Image', 'Convolution 3x3', 'Convolution 5x5']
images = [gray_img, convo1, convo2]

for i in range(len(images)):
    plt.subplot(1, 3, i+1)
    plt.imshow(images[i], cmap='gray')
    plt.title(title[i])
    plt.xticks([]), plt.yticks([])

plt.show()