# Morphological
import cv2
import matplotlib.pyplot as plt

img = cv2.imread("image/coin.jpg")  # โหลดภาพแบบสี
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # แปลงเป็น grayscale

# การ threshold แบบ binary inverse
thresh, result = cv2.threshold(gray_img, 230, 255, cv2.THRESH_BINARY_INV)

titles = ["Original", "Threshold"]
images = [gray_img, result]

for i in range(len(images)):  # ✅ ใช้ len(images)
    plt.subplot(1, 2, i+1)
    plt.imshow(images[i], cmap="gray")
    plt.title(titles[i])
    plt.xticks([]), plt.yticks([])  # ✅ ปิดวงเล็บให้ครบ

plt.show()
