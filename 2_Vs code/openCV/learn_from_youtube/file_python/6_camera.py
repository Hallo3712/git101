# เปิดกล้อง

import cv2

# กำหนด URL ของวิดีโอสตรีมจาก ESP32-CAM
# ต้องใส่ username และ password ถ้ามีการตั้งค่าไว้ เช่น "http://user:pass@192.168.1.104/stream"
cap = cv2.VideoCapture("http://192.168.1.104:81/stream") 

while (True):
    check , frame = cap.read() # อ่านภาพจากกล้อง
    cv2.imshow("output", frame) # แสดงภาพจากกล้อง

    #if cv2.waitKey(1) & 0xFF == ord("e"): # กดปุ่ม e เพื่อออกจากโปรแกรม
    #if cv2.waitKey(0): # รอการกดปุ่มใดๆ
    if cv2.waitKey(1) != -1: # กดปุ่มใดๆ เพื่อออกจากโปรแกรม
        break
cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ
