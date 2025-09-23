# การบันทึก วิดีโอจากกล้อง


import cv2

# กำหนด URL ของวิดีโอสตรีมจาก ESP32-CAM
cap = cv2.VideoCapture("http://192.168.1.104:81/stream") 
fourcc = cv2.VideoWriter_fourcc(*'XVID') # กำหนดฟอร์แมตของไฟล์ vdo ที่จะบันทึก
result = cv2.VideoWriter('output.avi', fourcc, 20.0, (640,480)) # กำหนดชื่อไฟล์ vdo , ฟอร์แมต , fps , ขนาด


while (cap.isOpened()): # ตรวจสอบว่าไฟล์ vdo เปิดได้หรือไม่
    check , frame = cap.read() # อ่านภาพจากกล้อง

    if check == True :
        cv2.imshow("output", frame) #แสดงภาพจากดล้อง
        result.write(frame) # บันทึกภาพลงไฟล์ vdo
        if cv2.waitKey(1) & 0xFF == ord("e"): # กดปุ่ม e เพื่อออกจากโปรแกรม 
                                              # ค่าในวงเล็บ cv2.waitKey(x) ถ้า x น้อย vdo จะเร็ว (18 ms = 1/60 วินาที)
          #if cv2.waitKey(0): # รอการกดปุ่มใดๆ vdo จะค้าง ไม่ยอมเล่น
          #if cv2.waitKey(1) != -1: # กดปุ่มใดๆ เพื่อออกจากโปรแกรม
            break
    else:
        break
result.release() # ปิดการบันทึกไฟล์ vdo
cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ

