# อ่าน vdo เป็นภาพขาวดำ (Gray Scale)

# เปิด vdo ด้วย openCV

import cv2

# กำหนด path ของไฟล์ vdo ที่ต้องการเปิด
# ต้องใส่ path ให้ถูกต้อง เช่น "C:\\Users\\PC6958\\Videos\\takraw.mp4"
cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\birdFlying.mp4")
#cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\takraw.mp4")

while (cap.isOpened()): # ตรวจสอบว่าไฟล์ vdo เปิดได้หรือไม่
    check , frame = cap.read() # อ่านภาพจากกล้อง

    if check == True :
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) # แปลงภาพเป็นขาวดำ
        cv2.imshow("output", gray) # แสดงภาพจากกล้อง
        if cv2.waitKey(1) & 0xFF == ord("e"): # กดปุ่ม e เพื่อออกจากโปรแกรม 
                                              # ค่าในวงเล็บ cv2.waitKey(x) ถ้า x น้อย vdo จะเร็ว (18 ms = 1/60 วินาที)
          #if cv2.waitKey(0): # รอการกดปุ่มใดๆ vdo จะค้าง ไม่ยอมเล่น
          #if cv2.waitKey(1) != -1: # กดปุ่มใดๆ เพื่อออกจากโปรแกรม
            break
    else:
        break

cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ

