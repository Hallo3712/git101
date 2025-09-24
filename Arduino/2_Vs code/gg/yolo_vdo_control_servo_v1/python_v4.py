import cv2
import keyboard
from ultralytics import YOLO

model = YOLO('yolov8n.pt')

video_path = r"C:\Users\PC6958\Videos\birdFlying.mp4"  # แก้ path

try:
    cap = cv2.VideoCapture(video_path)
except Exception as e:
    print(f"Error opening video file: {e}")
    exit()

if not cap.isOpened():
    print(f"Cannot open video file: {video_path}. Please check the file path and name.")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        print("End of video or an error occurred.")
        break

    results = model.track(frame, persist=True, classes=[14], conf=0.5, iou=0.5)

    for r in results:
        if hasattr(r, 'boxes') and r.boxes and r.boxes.id is not None:
            annotated_frame = r.plot()
            cv2.imshow("Real-Time Object Tracking", annotated_frame)
        else:
            cv2.imshow("Real-Time Object Tracking", frame)

    if keyboard.is_pressed('q'):
        print("Exiting application...")
        break
    elif keyboard.is_pressed('a'):
        print("Key 'A' pressed - Simulating servo movement to the left.")
    elif keyboard.is_pressed('d'):
        print("Key 'D' pressed - Simulating servo movement to the right.")
    elif keyboard.is_pressed(' '):
        print("Key 'Spacebar' pressed - Simulating servo stop.")

    cv2.waitKey(1)

cap.release()
cv2.destroyAllWindows()
