# sobel method 
import cv2
import matplotlib.pyplot as plt

# Load image and convert to grayscale
img = cv2.imread("image/coin.jpg")
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray_img = cv2.resize(gray_img, (600, 500))

sobelx = cv2.Sobel(gray_img,-1,1,0)
sobely = cv2.Sobel(gray_img,-1,0,1)
sobelxy = cv2.bitwise_or(sobelx, sobely)

images = [gray_img, sobelx, sobely, sobelxy]
titles = ['Gray Image', 'Sobel X', 'Sobel Y', 'Sobel XY']
for i in range(len(images)):
    plt.subplot(2, 2, i+1)
    plt.imshow(images[i], cmap='gray')
    plt.title(titles[i])
    plt.xticks([]), plt.yticks([])
plt.show()

