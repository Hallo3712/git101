# Contours
import cv2

img = cv2.imread("image/ant.jpg")
img = cv2.resize(img, (600, 500))

gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray_img = cv2.resize(gray_img, (600, 500))

thresh, result = cv2.threshold(gray_img, 201, 255, cv2.THRESH_BINARY)

contours, hierarchy = cv2.findContours(result, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
print("Number of contours found = " + str(len(contours)))


contours = cv2.drawContours(img, contours, -1, (0, 0, 255), 2)

# cv2.imshow("img",img)
cv2.imshow("result img", result)
cv2.imshow("contours", contours)
cv2.waitKey(0)
cv2.destroyAllWindows()