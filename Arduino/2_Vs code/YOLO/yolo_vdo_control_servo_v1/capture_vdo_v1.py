# Import the OpenCV library for video processing
import cv2
import os

# ==================== Configuration ====================
# The path to your video file
VIDEO_PATH = "takraw.mp4"

# The number of images you want to capture from the video
NUM_IMAGES_TO_CAPTURE = 20

# The output directory to save the captured images
# The full path to the 'images' folder for dataset annotation.
OUTPUT_DIR = r"C:\Users\PC6958\Desktop\TAWAN\Arduino\YOLO\yolo_vdo_control_servo_v1\takraw_dataset\images"

# =======================================================
def capture_frames_from_video(video_path, num_frames, output_dir):
    """
    Captures a specified number of frames from a video and saves them to a directory.

    Args:
        video_path (str): The path to the video file.
        num_frames (int): The number of frames to capture.
        output_dir (str): The directory to save the captured images.
    """
    # Create the output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Open the video file
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"Error: Could not open video file at {video_path}")
        return

    # Get video properties
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    print(f"Total frames in video: {total_frames}")

    # Calculate the interval to capture frames
    # We subtract 1 from total_frames and num_frames to ensure we get the first and last frame
    if num_frames > 1:
        frame_interval = total_frames / (num_frames - 1)
    else:
        # Handle case where only 1 frame is needed (the middle frame)
        frame_interval = total_frames / 2
        
    print(f"Capturing a frame every {frame_interval:.2f} frames.")

    # Loop to capture frames at calculated intervals
    for i in range(num_frames):
        # Calculate the frame number to capture
        frame_number = int(i * frame_interval)
        
        # Ensure we don't go past the last frame
        if frame_number >= total_frames:
            frame_number = total_frames - 1
            
        cap.set(cv2.CAP_PROP_POS_FRAMES, frame_number)
        
        # Read the frame
        ret, frame = cap.read()
        
        if ret:
            # Construct a filename
            filename = os.path.join(output_dir, f"frame_{frame_number:05d}.jpg")
            # Save the frame as an image
            cv2.imwrite(filename, frame)
            print(f"Saved frame {frame_number} to {filename}")
        else:
            print(f"Warning: Could not read frame {frame_number}")

    # Release the video capture object
    cap.release()
    print("Frame capture complete!")

# Call the function to start the process
if __name__ == "__main__":
    capture_frames_from_video(VIDEO_PATH, NUM_IMAGES_TO_CAPTURE, OUTPUT_DIR)
