# แยกสี
import cv2
import numpy

while True:
    
    img = cv2.imread("image/ball2d.png")
    img = cv2.resize(img, (400, 400))

    # green
    upper = numpy.array([23, 175, 16])
    lower = numpy.array([0, 50, 0])

    # blue
    #upper = numpy.array([130, 255, 255])
    #lower = numpy.array([100, 50, 50])

    # red
    #upper = numpy.array([180, 255, 255])
    #lower = numpy.array([160, 50, 50])

    mask = cv2.inRange(img , lower , upper)

    result = cv2.bitwise_and(img, img, mask=mask)


    cv2.imshow("19_Detect_Object_from_Color", img)
    cv2.imshow("Mask", mask)
    cv2.imshow("Result", result)

    if cv2.waitKey(1) & 0xFF == ord('e'):
        break
cv2.destroyAllWindows()





