#แสดงผลภาพด้วย matplotlib
import cv2
import matplotlib.pyplot as plt

img = cv2.imread("image/group_selfie.jpg")
cv2.imshow("Image", img)

img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  
plt.imshow(img)
plt.show()