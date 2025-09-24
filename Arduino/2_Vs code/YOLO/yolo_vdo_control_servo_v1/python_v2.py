import cv2
import time
from ultralytics import YOLO

# ==================== Configuration ====================
# Define video file path
VIDEO_PATH = 'takraw.mp4'

# Load the YOLO model.
model = YOLO('yolov8n.pt')

# Target FPS to control video playback speed
TARGET_FPS = 30
FRAME_TIME_MS = int(1000 / TARGET_FPS)

# Number of frames to skip when using 'a' and 'd' keys
FRAME_SKIP_COUNT = TARGET_FPS * 5  # Skip 5 seconds worth of frames

# Timeout for tracking persistence (in number of frames)
# Increased from 2 seconds to 4 seconds to improve tracking persistence
TRACKING_TIMEOUT_FRAMES = TARGET_FPS * 4 # Wait for 4 seconds before giving up

# ==================== Global Variables ====================
prev_y_pos = None
paused = False
tracked_object_id = None # Store the ID of the object we want to track
tracking_lost_counter = 0 # Counter for frames where the object is not found

# ==================== Functions ====================
def draw_info_on_frame(frame, boxes, tracked_id, paused_status):
    """Draws bounding boxes, IDs, and status messages on the video frame."""
    if tracked_id is None:
        cv2.putText(frame, "Press 's' to select an object to track.", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        # Draw all detected objects for selection
        for box in boxes:
            x1, y1, x2, y2 = [int(v) for v in box.xyxy[0]]
            track_id = int(box.id.item())
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, f"ID: {track_id}", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    else:
        # Draw only the tracked object
        found_target = False
        for box in boxes:
            if box.id.item() == tracked_id:
                x1, y1, x2, y2 = [int(v) for v in box.xyxy[0]]
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, f"Tracking ID: {tracked_id}", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
                found_target = True
                break
        
        if not found_target:
            # Check if tracking is temporarily lost but within the timeout period
            if tracking_lost_counter > 0 and tracking_lost_counter <= TRACKING_TIMEOUT_FRAMES:
                 cv2.putText(frame, f"Tracking lost... Waiting ({tracking_lost_counter}/{TRACKING_TIMEOUT_FRAMES})", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 165, 255), 2)
            else:
                 cv2.putText(frame, "Tracking lost. Press 's' to re-select.", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            
    if paused_status:
        cv2.putText(frame, "PAUSED", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)

    cv2.putText(frame, "Controls: q=Quit, s=Select, Space=Play/Pause, a/d=Skip 5s", (10, 470), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)


def handle_key_press(key, cap):
    """Handles keyboard commands for video control and tracking."""
    global paused, tracked_object_id, prev_y_pos, tracking_lost_counter

    if key == ord('q'): 
        return False
    elif key == ord(' '): 
        paused = not paused
        print(f"Video {'paused' if paused else 'resumed'}.")
    elif key == ord('d'):
        current_frame = cap.get(cv2.CAP_PROP_POS_FRAMES)
        cap.set(cv2.CAP_PROP_POS_FRAMES, current_frame + FRAME_SKIP_COUNT)
        print(f"Skipped forward by 5 seconds.")
    elif key == ord('a'):
        current_frame = cap.get(cv2.CAP_PROP_POS_FRAMES)
        cap.set(cv2.CAP_PROP_POS_FRAMES, max(0, current_frame - FRAME_SKIP_COUNT))
        print(f"Skipped backward by 5 seconds.")
    elif key == ord('s'):
        if tracked_object_id is None:
            paused = True
            print("Video paused for selection. Enter the ID of the object to track.")
            try:
                user_id = int(input("Enter ID: "))
                tracked_object_id = user_id
                print(f"Tracking object with ID {tracked_object_id}")
                prev_y_pos = None # Reset position to prevent a large jump
            except (ValueError, IndexError):
                print("Invalid input or no objects detected. Please try again.")
            finally:
                paused = False
                
    return True

def process_frame(frame):
    """Core logic to track a ball."""
    global prev_y_pos, tracked_object_id, tracking_lost_counter
    
    results = model.track(frame, persist=True, tracker='bytetrack.yaml', classes=32, verbose=False)
    boxes_with_ids = results[0].boxes if results and results[0].boxes.id is not None else []
    
    if tracked_object_id is not None:
        found_target = False
        for box in boxes_with_ids:
            if box.id.item() == tracked_object_id:
                found_target = True
                tracking_lost_counter = 0  # Reset the counter when the object is found
                
                x1, y1, x2, y2 = [int(v) for v in box.xyxy[0]]
                current_y_pos = y2
                
                # The servo control logic has been removed from this version
                
                prev_y_pos = current_y_pos
                break
        
        if not found_target:
            tracking_lost_counter += 1
            if tracking_lost_counter > TRACKING_TIMEOUT_FRAMES:
                tracked_object_id = None
                prev_y_pos = None
                print("Tracking lost. Press 's' to select a new object.")
                tracking_lost_counter = 0
        
    return boxes_with_ids

# ==================== Main Script ====================
def main():
    global tracked_object_id 
    
    cap = cv2.VideoCapture(VIDEO_PATH)
    if not cap.isOpened():
        print(f"Error: Could not open video file at {VIDEO_PATH}")
        return

    while True:
        if not paused:
            ret, frame = cap.read()
            if not ret:
                print("End of video. Looping...")
                cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                tracked_object_id = None
                continue
        
        # Resize frame for consistent processing
        frame = cv2.resize(frame, (640, 480))
        
        boxes = process_frame(frame)
        draw_info_on_frame(frame, boxes, tracked_object_id, paused)
        
        cv2.imshow('YOLO Video Analysis', frame)
        
        # Corrected handling of key press
        key = cv2.waitKey(FRAME_TIME_MS) & 0xFF
        if key == ord('q'):
            break  # Exit the loop on 'q' key press
        handle_key_press(key, cap)

    # Cleanup
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
