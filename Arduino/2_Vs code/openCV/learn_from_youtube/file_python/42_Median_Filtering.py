# Median Filtering 
import cv2
import numpy as np
import matplotlib.pyplot as plt

# Load image and convert to grayscale
img = cv2.imread("image/women_noise.png")
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# filter2d
filter2d = cv2.filter2D(gray_img, -1, np.ones((3,3), np.float32)/9)

# blur
mean = cv2.blur(gray_img, (5,5))

# median filter
mbur = cv2.medianBlur(gray_img, 5)


title = ['Gray Image', 'Filter2d', 'Mean Blur', 'Median Blur']
images = [gray_img, filter2d, mean, mbur]

for i in range(len(images)):
    plt.subplot(2, 2, i+1)
    plt.imshow(images[i], cmap='gray')
    plt.title(title[i])
    plt.xticks([]), plt.yticks([])

plt.show()