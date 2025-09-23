# Canny Method
import cv2

img = cv2.imread("image/coin.jpg")
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray_img = cv2.resize(gray_img, (600, 500))

canny =  cv2.Canny(gray_img, 50, 200)

cv2.imshow("gray img", gray_img)
cv2.imshow("canny img", canny)
cv2.waitKey(0)
cv2.destroyAllWindows()