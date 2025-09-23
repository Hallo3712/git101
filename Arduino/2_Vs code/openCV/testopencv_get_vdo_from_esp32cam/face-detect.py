import cv2 as cv
import time

esp32_url = "http://192.168.1.104:81/stream"

cap = cv.VideoCapture(esp32_url, cv.CAP_FFMPEG)

if not cap.isOpened():
    print("❌ ไม่สามารถเชื่อมต่อกับวิดีโอสตรีมได้")
    exit()

print("✅ กำลังประมวลผลวิดีโอจาก ESP32-CAM...")

time.sleep(2)  # รอให้การเชื่อมต่อสตรีมมีเวลา

while True:
    ret, frame = cap.read()
    if not ret:
        print("⚠️ ไม่สามารถอ่านภาพจากสตรีม")
        break

    # แปลงเป็น grayscale
    gray_scale = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    # โหลด Haar cascade สำหรับตรวจจับใบหน้า
    face_mode = cv.CascadeClassifier(cv.data.haarcascades + "haarcascade_frontalface_default.xml")
    faces = face_mode.detectMultiScale(gray_scale)

    # วาดกรอบ
    for (x, y, w, h) in faces:
        cv.rectangle(frame, (x, y), (x + w, y + h), (255, 255, 0), 2)

    # แสดงผล
    cv.imshow("ESP32-CAM Face Detection", frame)

    # กด 'q' เพื่อปิด
    if cv.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv.destroyAllWindows()
