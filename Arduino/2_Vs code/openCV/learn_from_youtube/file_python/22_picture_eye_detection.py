# ตรวจจับดวงตาในภาพ
import cv2

# อ่านภาพ
#img = cv2.imread("image/group_selfie.jpg")
img = cv2.imread("image/group_selfie.jpg")

# อ่านไฟล์สำหรับ classification
face_cascade = cv2.CascadeClassifier("file_xml/haarcascade_eye_tree_eyeglasses.xml")

# แปลงภาพเป็นขาวดำ
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# ตรวจจับดวงตาในภาพ
faces = face_cascade.detectMultiScale(gray, 1.02, 4)
# faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=4) 

# scaleFactor=1.1 คือ การลดขนาดภาพลง 10% ในแต่ละขั้นตอน
# scaleFactor = 1.1 

# minNeighbors=4 คือ จำนวนของเพื่อนบ้านที่ต้องการให้ตรวจจับได้ก่อน
# minNeighbors = 3 คือ

# วาดกรอบสี่เหลี่ยมรอบใบหน้า
for (x, y, w, h) in faces:
    cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

# แสดงภาพ
print("Number of faces detected:", len(faces))
#cv2.imshow("Gray", gray)
cv2.imshow("22_picture_eye_detection", img)
cv2.waitKey(0)
cv2.destroyAllWindows() 