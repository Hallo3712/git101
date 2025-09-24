
#### Step 1: ตรวจสอบการติดตั้ง
import torch
import ultralytics
from ultralytics import YOLO

print(f"PyTorch version: {torch.__version__}")
print(f"CUDA available: {torch.cuda.is_available()}")
print(f"Ultralytics version: {ultralytics.__version__}")

# ทดสอบโหลด model
model = YOLO('yolov8n.pt')
print("✅ YOLOv8 setup successful!")

