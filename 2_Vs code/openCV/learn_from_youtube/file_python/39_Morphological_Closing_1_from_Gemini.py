# Image Processing with Morphological Operations
import cv2
import matplotlib.pyplot as plt
import numpy as np

# โหลดภาพและแปลงเป็น grayscale
img = cv2.imread("image/coin.jpg")
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# การ threshold แบบ binary inverse
thresh, result = cv2.threshold(gray_img, 230, 255, cv2.THRESH_BINARY_INV)

# สร้าง kernel สำหรับการทำ morphological operation
kernel = np.ones((2, 2), np.uint8)

# กำหนดจำนวนรอบในการทำ operation
my_iter = 5

# การกร่อนภาพ (Erosion)
erosion = cv2.erode(result, kernel, iterations=my_iter)

# การขยายภาพ (Dilation)
dilation = cv2.dilate(result, kernel, iterations=my_iter)

# การทำ Opening
opening = cv2.morphologyEx(result, cv2.MORPH_OPEN, kernel, iterations=my_iter)

# การทำ Closing
closing = cv2.morphologyEx(result, cv2.MORPH_CLOSE, kernel, iterations=my_iter)

# สร้าง list ของชื่อภาพและตัวแปรภาพสำหรับแสดงผล
titles = ["Original", "Threshold", "Erosion", "Dilation", "Opening", "Closing"]
images = [gray_img, result, erosion, dilation, opening, closing]

# แสดงผลภาพทั้งหมด
# ปรับขนาด figure ให้เล็กลงจาก (15, 10) เป็น (12, 8)
plt.figure(figsize=(10, 6))
for i in range(len(images)):
    plt.subplot(2, 3, i + 1)
    plt.imshow(images[i], cmap="gray")
    plt.title(titles[i])
    plt.xticks([]), plt.yticks([])

plt.tight_layout()
plt.show()