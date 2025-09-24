import cv2
import requests
import time
from ultralytics import YOLO

# Define video file path and ESP8266 IP address
video_path = 'jumping_rope.mp4'
esp_ip = "192.168.1.105"

# Load the YOLO model
model = YOLO('yolov8n.pt')

# Global variables for tracking and video control
prev_y_pos = None
paused = False
tracked_person_id = None # Store the ID of the person we want to track

# Target FPS
target_fps = 30
frame_time = 1.0 / target_fps

def send_angle(angle):
    """Function to send an angle value to the ESP8266."""
    try:
        requests.get(f"http://{esp_ip}/servo_control?angle={angle}", timeout=1.0)
        print(f"Sent angle: {angle}")
    except requests.exceptions.RequestException as e:
        print(f"Failed to send angle: {e}")

# Create a video capture object
cap = cv2.VideoCapture(video_path)
if not cap.isOpened():
    print("Error: Could not open video file.")
    exit()

# Loop to process video frames
while True:
    start_time = time.time()
    
    if not paused:
        ret, frame = cap.read()
        if not ret:
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
            tracked_person_id = None
            continue
    
    frame = cv2.resize(frame, (640, 480))

    # Perform multi-object tracking using YOLO
    results = model.track(frame, persist=True, tracker='bytetrack.yaml', classes=0, verbose=False)
    
    boxes_with_ids = results[0].boxes if results and results[0].boxes.id is not None else []
    
    if tracked_person_id is None:
        cv2.putText(frame, "Press 's' to select a person to track.", (100, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (255, 255, 255), 2)
        # Always draw boxes and IDs when tracking is not active
        for box in boxes_with_ids:
            x1, y1, x2, y2 = [int(v) for v in box.xyxy[0]]
            track_id = int(box.id.item())
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, f"ID: {track_id}", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    else:
        found_target = False
        for box in boxes_with_ids:
            if box.id.item() == tracked_person_id:
                found_target = True
                
                x1, y1, x2, y2 = [int(v) for v in box.xyxy[0]]
                current_y_pos = y2

                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.circle(frame, (x1 + (x2 - x1) // 2, y2), 5, (0, 0, 255), -1)

                if prev_y_pos is not None:
                    y_movement = prev_y_pos - current_y_pos
                    max_jump_height = 50 
                    servo_angle = int((y_movement - 0) * (180 - 0) / (max_jump_height - 0) + 0)
                    servo_angle = max(0, min(180, servo_angle))
                    send_angle(servo_angle)
                
                prev_y_pos = current_y_pos
                break
        
        if not found_target:
            # THIS IS THE FIX: Set tracked_person_id to None to re-enable the 's' key
            tracked_person_id = None
            prev_y_pos = None
            print("Tracking lost. Press 's' to select a new person.")
            cv2.putText(frame, "Tracking lost. Press 's' to select.", (100, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 2)

    cv2.imshow('Video Analysis', frame)
    
    end_time = time.time()
    elapsed_time = end_time - start_time
    if elapsed_time < frame_time:
        time_to_wait = int((frame_time - elapsed_time) * 1000)
        if time_to_wait > 0:
            key = cv2.waitKey(time_to_wait) & 0xFF
        else:
            key = cv2.waitKey(1) & 0xFF
    else:
        key = cv2.waitKey(1) & 0xFF
    
    if key == ord('q'): 
        break
    elif key == ord(' '): 
        paused = not paused
        if paused:
            print("Video paused.")
        else:
            print("Video resumed.")
    elif key == ord('d'):
        current_frame = cap.get(cv2.CAP_PROP_POS_FRAMES)
        cap.set(cv2.CAP_PROP_POS_FRAMES, current_frame + 25 * 10)
    elif key == ord('a'):
        current_frame = cap.get(cv2.CAP_PROP_POS_FRAMES)
        cap.set(cv2.CAP_PROP_POS_FRAMES, max(0, current_frame - 25 * 10))
    elif key == ord('s') and tracked_person_id is None:
        paused = True
        
        results = model.track(frame, persist=True, tracker='bytetrack.yaml', verbose=False)
        
        if results[0].boxes.id is not None:
            track_ids = results[0].boxes.id.int().cpu().tolist()
            if track_ids:
                print("Select a person by entering their ID and pressing Enter.")
                try:
                    user_id = int(input("Enter the ID of the person to track: "))
                    if user_id in track_ids:
                        tracked_person_id = user_id
                        print(f"Tracking person with ID {tracked_person_id}")
                    else:
                        print("Invalid ID. Please try again.")
                except ValueError:
                    print("Invalid input. Please enter a number.")
            else:
                print("No people detected in this frame. Please try again.")
        else:
            print("No people detected in this frame. Please try again.")
        
        paused = False

# Cleanup
cap.release()
cv2.destroyAllWindows()