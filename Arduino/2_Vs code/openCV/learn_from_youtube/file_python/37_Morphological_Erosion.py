# Erosion Morphological Operation
import cv2
import matplotlib.pyplot as plt
import numpy as np

img = cv2.imread("image/coin.jpg")  # โหลดภาพแบบสี
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # แปลงเป็น grayscale

# การ threshold แบบ binary inverse
thresh, result = cv2.threshold(gray_img, 170, 255, cv2.THRESH_BINARY_INV)
kernel = np.ones((2,2), np.uint8)  # kernel ขนาด 5x5

# การขยายภาพ (Dilation)
dilation = cv2.dilate(result, kernel, iterations=5)  # ทำการ dilation

# การกร่อนภาพ (Erosion)
erosion = cv2.erode(dilation, kernel, iterations=5)  # ทำการ erosion


titles = ["Original", "Threshold", "Dilation", "Erosion"]
images = [gray_img, result, dilation, erosion]

for i in range(len(images)):  # ✅ ใช้ len(images)
    plt.subplot(2,2, i+1)
    plt.imshow(images[i], cmap="gray")
    plt.title(titles[i])
    plt.xticks([]), plt.yticks([])  # ✅ ปิดวงเล็บให้ครบ

plt.show()
