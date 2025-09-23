# การปรับ Threshold ด้วย Trackbar
import cv2
import matplotlib.pyplot as plt

def display(value):
    pass

cv2.namedWindow("Trackbar Window")
# cv2.createTrackbar("Threshold", "Trackbar Window", 0, 255, lambda x: None)
cv2.createTrackbar("Threshold", "Trackbar Window", 128, 255, display)

while True:
    gray_img = cv2.imread("image/ant.jpg", 0)  # โหลดภาพแบบ grayscale
    thresvalue = cv2.getTrackbarPos("Threshold", "Trackbar Window")
    thres,result = cv2.threshold(gray_img, thresvalue, 255, cv2.THRESH_BINARY)
    if cv2.waitKey(1) & 0xFF == ord('e'):
        break
    cv2.imshow("Trackbar Window", result)
cv2.destroyAllWindows()



