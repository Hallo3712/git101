# การใช้ Adaptive Thresholding
import cv2
import matplotlib.pyplot as plt

img = cv2.imread("image/group_selfie.jpg")  # โหลดภาพแบบสี
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # แปลงเป็น grayscale

# กำหนดขนาด block size
size = [3,5,9,17,33]

plt.subplot(231,xticks=[],yticks=[])
plt.imshow(gray_img,cmap="gray")

for i in range(len(size)):
    result = cv2.adaptiveThreshold(gray_img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,size[i],1)
    plt.subplot(232+i)
    plt.title("%d"%size[i])
    plt.imshow(result,cmap="gray")
    plt.xticks([]),plt.yticks([])

plt.show()




