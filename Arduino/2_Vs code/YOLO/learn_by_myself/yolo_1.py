from ultralytics import YOLO
from matplotlib import pyplot as plt
import cv2
#%matplotlib inline


# โหลดโมเดล YOLO
model = YOLO(r"C:\Users\PC6958\Desktop\TAWAN\Arduino\2_Vs code\YOLO\yolov8n.pt")

# เส้นทางรูปภาพ
img_path = ["C:/Users/PC6958/Desktop/TAWAN/Arduino/2_Vs code/YOLO/learn_yolo_from_youtube/PythonComputerVision-main/PythonComputerVision-main/YOLO-Detection/datasets/coco8/images/1.jpg"]
img_path = ["C:/Users/PC6958/Desktop/TAWAN/Arduino/2_Vs code/YOLO/learn_yolo_from_youtube/PythonComputerVision-main/PythonComputerVision-main/YOLO-Detection/datasets/coco8/images/"]
# Run inference with the YOLO12n model on the 'bus.jpg' image
results = model(source=img_path, save=False, device='cpu', conf=0.3) # ถ้ามี webcam ให้ใส่ source=0, ถ้ามีการ์ดจอ device=0
for r in results:
    annotated = r.plot() 
    cv2.imshow("Detection Result", annotated)
    cv2.waitKey(-1)
    cv2.destroyAllWindows()
