# ตรวจจับ pixel สี แล้วเปิดหน้าต่างแสดงผล
import cv2
import numpy 

# อ่านภาพ
img = cv2.imread("image/cat.jpg")

# สร้างหน้าต่างและแสดงภาพ
cv2.imshow("16_Detect_color_1", img)

# ฟังก์ชันสำหรับ Mouse Event
def clickPosition(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:  # ถ้าคลิกซ้าย
        blue = img[y, x, 0]
        green = img[y, x, 1]
        red = img[y, x, 2]
        imgcolor = numpy.zeros([300,300,3],numpy.uint8)
        imgcolor[:] = [blue, green, red]
        cv2.imshow("result", imgcolor)

    elif event == cv2.EVENT_RBUTTONDOWN:  # ถ้าคลิกขวา
        text = f"({x},{y})"
        cv2.putText(img, "poit " + text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        cv2.imshow("16_Detect_color_1", img)

    elif event == cv2.EVENT_MBUTTONDOWN:  # ถ้าคลิกกลาง
        cv2.putText(img, "Middle", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        cv2.imshow("16_Detect_color_1", img)

# ตั้งค่า Mouse Callback ให้กับหน้าต่างที่สร้างขึ้น
cv2.setMouseCallback("16_Detect_color_1", clickPosition)

# รอการกดคีย์และปิดหน้าต่างทั้งหมด
cv2.waitKey(0)
cv2.destroyAllWindows()
