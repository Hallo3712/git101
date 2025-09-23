from flask import Flask, render_template, Response, request
import cv2
import threading
import time
import os

app = Flask(__name__)

# Global variables
cap = None
tracker = None
is_tracking = False
is_paused = False
current_frame = None
video_file_path = None
roi_set = False
is_seeking = False

# Function to run video processing in a separate thread
def process_video():
    global cap, tracker, is_tracking, is_paused, current_frame, video_file_path, roi_set

    # Wait for the video file to be available
    while video_file_path is None or not os.path.exists(video_file_path):
        time.sleep(1)

    cap = cv2.VideoCapture(video_file_path)
    if not cap.isOpened():
        print("Error: Could not open video file.")
        return

    fps = cap.get(cv2.CAP_PROP_FPS)
    frame_time = 1 / fps if fps > 0 else 0.033

    while cap.isOpened():
        start_time = time.time()
        
        if not is_paused:
            ret, frame = cap.read()
            if not ret:
                cap.release()
                break
            current_frame = frame
        
        display_frame = current_frame.copy()
        
        # Tracking logic
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
        
        # If ROI is set but tracking is not started yet, draw the initial ROI
        elif not is_tracking and roi_set:
            (x, y, w, h) = [int(v) for v in roi_set]
            cv2.rectangle(display_frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

        # Encode frame to JPG
        ret, buffer = cv2.imencode('.jpg', display_frame)
        if not ret:
            continue
        
        frame_bytes = buffer.tobytes()
        
        # This is for streaming. We don't display with imshow anymore.
        current_frame = display_frame
        
        # FPS control
        elapsed_time = time.time() - start_time
        time_to_wait = frame_time - elapsed_time
        if time_to_wait > 0:
            time.sleep(time_to_wait)

    print("Video processing stopped.")

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/upload_video', methods=['POST'])
def upload_video():
    global video_file_path
    if 'video' not in request.files:
        return "No video file provided.", 400
    
    video_file = request.files['video']
    video_file_path = "uploaded_video.mp4"
    video_file.save(video_file_path)
    
    # Start the processing thread if not already running
    if not any(t.name == "video_thread" for t in threading.enumerate()):
        threading.Thread(target=process_video, name="video_thread").start()
    
    return "Video uploaded successfully."

@app.route('/video_feed')
def video_feed():
    def generate_frames():
        global current_frame
        while True:
            if current_frame is not None:
                ret, buffer = cv2.imencode('.jpg', current_frame)
                frame_bytes = buffer.tobytes()
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')
            time.sleep(0.01)

    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/command/<cmd>')
def handle_command(cmd):
    global is_paused, is_tracking, tracker, cap, roi_set, is_seeking 

    if cmd == 'play':
        is_paused = False
    elif cmd == 'pause':
        is_paused = True
    elif cmd == 'toggle_tracking':
        is_tracking = not is_tracking
        if is_tracking and cap is not None and roi_set:
            if tracker is None:
                tracker = cv2.TrackerCSRT_create()
                tracker.init(current_frame, roi_set)
            print("Tracking started.")
        elif not is_tracking:
            tracker = None
            print("Tracking stopped.")

    elif cmd == 'seek_forward':
        if not is_seeking and cap is not None:
            is_seeking = True
            current_pos_ms = cap.get(cv2.CAP_PROP_POS_MSEC)
            total_duration_ms = cap.get(cv2.CAP_PROP_FRAME_COUNT) / cap.get(cv2.CAP_PROP_FPS) * 1000
            new_pos_ms = min(total_duration_ms, current_pos_ms + 10000)
            cap.set(cv2.CAP_PROP_POS_MSEC, new_pos_ms)
            is_tracking = False
            is_seeking = False # รีเซ็ตสถานะเมื่อเสร็จ

    elif cmd == 'seek_backward':
        if not is_seeking and cap is not None:
            is_seeking = True
            current_pos_ms = cap.get(cv2.CAP_PROP_POS_MSEC)
            new_pos_ms = max(0, current_pos_ms - 10000)
            
            if new_pos_ms < 500:
                 cap.set(cv2.CAP_PROP_POS_MSEC, 0)
            else:
                 cap.set(cv2.CAP_PROP_POS_MSEC, new_pos_ms)

            is_tracking = False
            is_seeking = False # รีเซ็ตสถานะเมื่อเสร็จ
    elif cmd == 'set_roi':
        # Use a predefined ROI for demonstration
        roi_set = (100, 100, 150, 150) # x, y, width, height
        print(f"ROI set to {roi_set}. Press 'Toggle Tracking' to start.")
    
    return "OK"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)