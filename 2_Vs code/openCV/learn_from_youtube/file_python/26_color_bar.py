#การสร้าง Track bar เบื้องต้น
import cv2
import numpy as np

img = np.zeros((200,400,3), np.uint8)

cv2.namedWindow("Track Bar")

def display(value):
    print(value)

#เริ่มต้นสร้าง color trackbar
cv2.createTrackbar("R", "Track Bar", 0, 255, display)
cv2.createTrackbar("G", "Track Bar", 0, 255, display)
cv2.createTrackbar("B", "Track Bar", 0, 255, display)

while True:
    cv2.imshow("Track Bar", img)
    if cv2.waitKey(1) & 0xFF == ord("e"): 
        break

    r = cv2.getTrackbarPos("R", "Track Bar")
    g = cv2.getTrackbarPos("G", "Track Bar")
    b = cv2.getTrackbarPos("B", "Track Bar")

    img[:] = [b, g, r]

#cv2.waitKey(0)
cv2.destroyAllWindows()
