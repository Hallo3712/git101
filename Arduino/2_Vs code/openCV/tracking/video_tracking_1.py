import cv2
import time

video_path = 'C:/Users/PC6958/Videos/takraw.mp4'
cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    print("Error: Could not open video file.")
    exit()

fps = cap.get(cv2.CAP_PROP_FPS)
frame_time = 1 / fps if fps > 0 else 0.033

tracker = None
roi = None
is_tracking = False
is_paused = False

while True:
    start_time = time.time()

    if not is_paused:
        ret, frame = cap.read()
        if not ret:
            break
    else:
        # ถ้าหยุดอยู่ ก็ใช้เฟรมเดิม
        pass
    
    display_frame = frame.copy()
    
    if is_tracking:
        success, box = tracker.update(display_frame)
        if success:
            (x, y, w, h) = [int(v) for v in box]
            cv2.rectangle(display_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.circle(display_frame, (x + w // 2, y + h // 2), 5, (0, 0, 255), -1)
        else:
            is_tracking = False
            tracker = None
            print("Tracking lost.")

    cv2.imshow('Video Tracker', display_frame)

    elapsed_time = time.time() - start_time
    wait_time = max(1, int((frame_time - elapsed_time) * 1000))
    key = cv2.waitKey(wait_time) & 0xFF

    if key == ord('s'):
        is_paused = True
        roi = cv2.selectROI('Video Tracker', frame, showCrosshair=False)
        is_paused = False
        
        # เมื่อเลือก ROI เสร็จแล้ว ให้เริ่มต้นการ tracking ทันที
        if roi and roi != (0, 0, 0, 0):
            tracker = cv2.TrackerCSRT_create()
            tracker.init(frame, roi)
            is_tracking = True
            print("Tracking started immediately after ROI selection.")

    elif key == ord(' '):
        if is_tracking:
            is_tracking = False
            tracker = None
            print("Tracking stopped.")
        else:
            is_paused = not is_paused

    elif key == ord('a'):
        current_pos_ms = cap.get(cv2.CAP_PROP_POS_MSEC)
        new_pos_ms = max(0, current_pos_ms - 10000)
        cap.set(cv2.CAP_PROP_POS_MSEC, new_pos_ms)
        is_tracking = False

    elif key == ord('d'):
        current_pos_ms = cap.get(cv2.CAP_PROP_POS_MSEC)
        new_pos_ms = current_pos_ms + 10000
        cap.set(cv2.CAP_PROP_POS_MSEC, new_pos_ms)
        is_tracking = False

    elif key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()