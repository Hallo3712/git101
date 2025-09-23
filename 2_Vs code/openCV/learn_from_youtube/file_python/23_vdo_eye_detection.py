# ตรวจจับดวงตาในไฟล์ vdo
import cv2

# กำหนด path ของไฟล์ vdo ที่ต้องการเปิด
cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\warren_buffett.mp4")

# อ่านไฟล์สำหรับ classification
face_cascade = cv2.CascadeClassifier("file_xml/haarcascade_eye_tree_eyeglasses.xml")

while (cap.isOpened()): # ตรวจสอบว่าไฟล์ vdo เปิดได้หรือไม่
    check , frame = cap.read() # อ่านภาพจากกล้อง
    
    if check == True :
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) # แปลงภาพเป็นขาวดำ

        # ตรวจจับใบหน้า
        faces = face_cascade.detectMultiScale(gray, 1.1,3)#, minSize=(60, 60),maxSize=(150, 150))
        # faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=4)
        print("Number of faces detected:", len(faces))

        # วาดกรอบสี่เหลี่ยมรอบใบหน้า
        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)


        cv2.imshow("21_Vdo_Face_Detect", frame) # แสดงภาพจากกล้อง
        if cv2.waitKey(20) != -1: # กดปุ่มใดๆ เพื่อออกจากโปรแกรม
            break
    else:
        break

cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ
