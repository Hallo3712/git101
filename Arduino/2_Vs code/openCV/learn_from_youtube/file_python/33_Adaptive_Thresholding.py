# การใช้ Adaptive Thresholding
import cv2

img = cv2.imread("image/group_selfie.jpg")  # โหลดภาพแบบสี
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # แปลงเป็น grayscale

thresh,th1 = cv2.threshold(gray_img,128,255,cv2.THRESH_BINARY)

th2 = cv2.adaptiveThreshold(gray_img,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,3,1)
th3 = cv2.adaptiveThreshold(gray_img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,3,1)

weight = 400
height = 350

img = cv2.resize(img, (weight, height))
th1 = cv2.resize(th1, (weight, height))
th2 = cv2.resize(th2, (weight, height))
th3 = cv2.resize(th3, (weight, height))

cv2.imshow("img",img)
cv2.imshow("img_original",th1)
cv2.imshow("Mean",th2)
cv2.imshow("GAUSSIAN",th3)

cv2.waitKey(0)
cv2.destroyAllWindows()


