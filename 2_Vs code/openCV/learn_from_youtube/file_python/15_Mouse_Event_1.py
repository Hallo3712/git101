# แสดงพิกัดด้วย Mouse Event และลบข้อความหลังจาก 2 วินาที
import cv2

# ====== ตั้งค่าระยะเวลาที่แสดงข้อความ (หน่วยเป็นมิลลิวินาที) ======
DISPLAY_TIME_MS = 500  # 2000 ms = 2 วินาที
# ===============================================================

# อ่านภาพและเก็บภาพต้นฉบับไว้
img_original = cv2.imread("image/cat.jpg")
img = img_original.copy()

# สร้างหน้าต่างและแสดงภาพ
cv2.imshow("15_Mouse_Event", img)

# ฟังก์ชันสำหรับ Mouse Event
def clickPosition(event, x, y, flags, param):
    global img
    if event == cv2.EVENT_LBUTTONDOWN:  # คลิกซ้าย
        img = img_original.copy()
        cv2.putText(img, "OK", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.imshow("15_Mouse_Event", img)
        cv2.waitKey(DISPLAY_TIME_MS)
        img = img_original.copy()
        cv2.imshow("15_Mouse_Event", img)

    elif event == cv2.EVENT_RBUTTONDOWN:  # คลิกขวา
        img = img_original.copy()
        text = f"({x},{y})"
        cv2.putText(img, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        cv2.imshow("15_Mouse_Event", img)
        cv2.waitKey(DISPLAY_TIME_MS)
        img = img_original.copy()
        cv2.imshow("15_Mouse_Event", img)

    elif event == cv2.EVENT_MBUTTONDOWN:  # คลิกกลาง
        img = img_original.copy()
        cv2.putText(img, "Middle", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
        cv2.imshow("15_Mouse_Event", img)
        cv2.waitKey(DISPLAY_TIME_MS)
        img = img_original.copy()
        cv2.imshow("15_Mouse_Event", img)

# ตั้งค่า Mouse Callback
cv2.setMouseCallback("15_Mouse_Event", clickPosition)

# รอการกดคีย์และปิดหน้าต่าง
cv2.waitKey(0)
cv2.destroyAllWindows()
