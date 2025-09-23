# compare threshold value in matplotlib
import cv2
import matplotlib.pyplot as plt

# Load image and convert to grayscale
img = cv2.imread("image/group_selfie.jpg")  # โหลดภาพแบบสี
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # แปลงเป็น grayscale

tresh_value = [50, 100, 150, 200, 250]
plt.subplot(2, 3, 1) # plt.subplot(2,3,1) # 2 rows, 3 columns
plt.imshow(gray_img, cmap='gray')
plt.title('Gray Image')
plt.xticks([]), plt.yticks([])  # Hide tick marks 

for i in range(len(tresh_value)):
    thresh, result = cv2.threshold(gray_img, tresh_value[i], 255, cv2.THRESH_BINARY)
    plt.subplot(2, 3, i + 2)  # 2 rows, 3 columns
    plt.imshow(result, cmap='gray')
    plt.title('Threshold = ' + str(tresh_value[i]))
    plt.xticks([]), plt.yticks([])  # Hide tick marks

plt.show()
