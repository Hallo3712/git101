# สร้างเส้นเชื่อมโยง
import cv2
import numpy 

#img = numpy.zeros([400, 400, 3])
img = cv2.imread("image/cat.jpg")

points = []  # เก็บพิกัดที่คลิก

# ฟังก์ชันสำหรับ Mouse Event
def clickPosition(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:  # ถ้าคลิกซ้าย
        cv2.circle(img, (x, y), 10, (0, 0, 255), 5)  # วาดวงกลมสีแดง
        points.append((x, y)) # เก็บพิกัดที่คลิก
        
        if len(points) >= 2:  # ถ้ามีจุดมากกว่าหรือเท่ากับ 2 จุด
            cv2.line(img, points[-1], points[-2], (0, 255, 0), 5)
        print(points)
        cv2.imshow("18_Connect_point", img) # แสดงภาพที่อัปเดต


# สร้างหน้าต่างและแสดงภาพ
cv2.imshow("18_Connect_point", img)

# ตั้งค่า Mouse Callback ให้กับหน้าต่างที่สร้างขึ้น
cv2.setMouseCallback("18_Connect_point", clickPosition)

# รอการกดคีย์และปิดหน้าต่างทั้งหมด
cv2.waitKey(0)
cv2.destroyAllWindows()
