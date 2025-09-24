import cv2
import keyboard # นำเข้าไลบรารี 'keyboard' ที่จะใช้ในการตรวจจับการกดปุ่ม
from ultralytics import YOLO

# โหลดโมเดล YOLOv8 ที่ถูกฝึกมาแล้ว
model = YOLO('yolov8n.pt')

# เปิดไฟล์วิดีโอ 'birdFlying.mp4'
video_path = "C:\Users\PC6958\Videos\birdFlying.mp4"
try:
    cap = cv2.VideoCapture(video_path)
except Exception as e:
    print(f"Error opening video file: {e}")
    exit()

# ตรวจสอบว่าไฟล์วิดีโอเปิดได้สำเร็จหรือไม่
if not cap.isOpened():
    print(f"Cannot open video file: {video_path}. Please check the file path and name.")
    exit()

# วนลูปเพื่อประมวลผลแต่ละเฟรมของวิดีโอ
while True:
    # อ่านเฟรมจากวิดีโอ
    ret, frame = cap.read()
    if not ret:
        print("End of video or an error occurred.")
        break

    # รันโมเดล YOLOv8 บนเฟรมเพื่อติดตามวัตถุ
    # เราติดตามคลาส 14 ซึ่งเป็นคลาสสำหรับ 'bird' (นก) ใน COCO dataset
    results = model.track(frame, persist=True, classes=[14], stream=True, conf=0.5, iou=0.5)

    # วนลูปในผลลัพธ์เพื่อวาดกรอบสี่เหลี่ยมรอบวัตถุ
    for r in results:
        if hasattr(r, 'boxes') and r.boxes and r.boxes.id is not None:
            # ฟังก์ชัน .plot() จะวาดกรอบ, ชื่อคลาส และคะแนนความมั่นใจ
            annotated_frame = r.plot()
            cv2.imshow("Real-Time Object Tracking", annotated_frame)
        else:
            # หากไม่มี ID การติดตาม ให้แสดงเฟรมต้นฉบับ
            cv2.imshow("Real-Time Object Tracking", frame)
    
    # **ใช้ไลบรารี 'keyboard' สำหรับการตรวจจับการกดปุ่ม**
    # วิธีนี้จะทำงานได้ไม่ว่าหน้าต่าง OpenCV จะมีโฟกัสหรือไม่ก็ตาม
    if keyboard.is_pressed('q'):
        print("Exiting application...")
        break
    elif keyboard.is_pressed('a'):
        print("Key 'A' pressed - Simulating servo movement to the left.")
    elif keyboard.is_pressed('d'):
        print("Key 'D' pressed - Simulating servo movement to the right.")
    elif keyboard.is_pressed(' '):
        print("Key 'Spacebar' pressed - Simulating servo stop.")

    # cv2.waitKey(1) ยังคงจำเป็นสำหรับการประมวลผลการแสดงผลของหน้าต่าง OpenCV
    cv2.waitKey(1)

# ปล่อยการจับภาพวิดีโอและปิดหน้าต่างทั้งหมด
cap.release()
cv2.destroyAllWindows()
