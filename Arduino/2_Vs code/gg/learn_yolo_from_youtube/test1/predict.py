from ultralytics import YOLO
import torch

print(f"✅ PyTorch version: {torch.__version__}")
print(f"✅ CUDA available: {torch.cuda.is_available()}")

model = YOLO('yolov8n.pt')  # ทดสอบโหลดโมเดล (โหลดอัตโนมัติถ้ายังไม่มี)
print("🎉 YOLO ติดตั้งสำเร็จ!")