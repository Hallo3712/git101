# Threshold
import cv2
import matplotlib.pyplot as plt

# Set width and height for resizing images
width = 300
height = 300

# Load image and convert to grayscale
img = cv2.imread("image/gradient_1.jpg", 1)  # โหลดภาพแบบสี
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # แปลงเป็น grayscale

# Apply different thresholding methods
thresh1,result1 = cv2.threshold(gray_img, 128, 255, cv2.THRESH_BINARY)      #threh_bunary คือ ถ้าค่ามากกว่า 128 ให้เป็น 255 ถ้าน้อยกว่าให้เป็น 0
thresh2,result2 = cv2.threshold(gray_img, 128, 255, cv2.THRESH_BINARY_INV)  #threh_bunary_inv คือ ถ้าค่ามากกว่า 128 ให้เป็น 0 ถ้าน้อยกว่าให้เป็น 255
thresh3,result3 = cv2.threshold(gray_img, 128, 255, cv2.THRESH_TRUNC)       #threh_trunc คือ ถ้าค่ามากกว่า 128 ให้เป็น 128 ถ้าน้อยกว่าให้เป็นค่าตัวเอง
thresh4,result4 = cv2.threshold(gray_img, 128, 255, cv2.THRESH_TOZERO)      #threh_tozero คือ ถ้าค่ามากกว่า 128 ให้เป็นค่าตัวเอง ถ้าน้อยกว่าให้เป็น 0
thresh5,result5 = cv2.threshold(gray_img, 128, 255, cv2.THRESH_TOZERO_INV)  #threh_tozero_inv คือ ถ้าค่ามากกว่า 128 ให้เป็น 0 ถ้าน้อยกว่าให้เป็นค่าตัวเอง

# แสดงค่า threshold
print("Threshold1: ", thresh1)
print("Threshold2: ", thresh2)
print("Threshold3: ", thresh3)
print("Threshold4: ", thresh4)
print("Threshold5: ", thresh5)

# ปรับขนาดภาพให้เท่ากัน
img = cv2.resize(img, (width, height))
gray_img = cv2.resize(gray_img, (width, height))
result1 = cv2.resize(result1, (width, height))
result2 = cv2.resize(result2, (width, height))
result3 = cv2.resize(result3, (width, height))
result4 = cv2.resize(result4, (width, height))
result5 = cv2.resize(result5, (width, height))

# แสดงผลภาพ
cv2.imshow("Image", img)
cv2.imshow("Gray", gray_img)
cv2.imshow("Result1", result1)
cv2.imshow("Result1", result1)
cv2.imshow("Result2", result2)
cv2.imshow("Result3", result3)
cv2.imshow("Result4", result4)
cv2.imshow("Result5", result5)


cv2.waitKey(0)
cv2.destroyAllWindows()