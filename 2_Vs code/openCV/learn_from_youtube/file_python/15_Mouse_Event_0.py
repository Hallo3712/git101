# แสดงพิกัดด้วย mouse event
import cv2

# อ่านภาพ
img = cv2.imread("image/cat.jpg")

# สร้างหน้าต่างและแสดงภาพ
cv2.imshow("15_Mouse_Event", img)

# ฟังก์ชันสำหรับ Mouse Event
def clickPosition(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:  # ถ้าคลิกซ้าย
        cv2.putText(img, "OK", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.imshow("15_Mouse_Event", img)

    elif event == cv2.EVENT_RBUTTONDOWN:  # ถ้าคลิกขวา
        text = f"({x},{y})"
        cv2.putText(img, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        cv2.imshow("15_Mouse_Event", img)

    elif event == cv2.EVENT_MBUTTONDOWN:  # ถ้าคลิกกลาง
        cv2.putText(img, "Middle", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
        cv2.imshow("15_Mouse_Event", img)

# ตั้งค่า Mouse Callback ให้กับหน้าต่างที่สร้างขึ้น
cv2.setMouseCallback("15_Mouse_Event", clickPosition)

# รอการกดคีย์และปิดหน้าต่างทั้งหมด
cv2.waitKey(0)
cv2.destroyAllWindows()
