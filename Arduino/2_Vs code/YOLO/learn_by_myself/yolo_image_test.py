from ultralytics import YOLO
import cv2

model = YOLO('yolov8n.pt') 

source = "D:/Pictures/ant.jpg"

# รัน predict โดยเอา show=False ออก เพื่อไม่ให้ Ultralytics แสดงผลเอง
results = model.predict(source, show=False)

# ดึงภาพที่ Ultralytics ทำการวาดกรอบให้แล้ว
# .plot() จะแปลงผลลัพธ์ให้เป็นภาพในรูปแบบ Numpy array
result_image = results[0].plot()

# ใช้ cv2.imshow เพื่อแสดงภาพนั้น
cv2.imshow("Output", result_image)

cv2.waitKey(0)
cv2.destroyAllWindows()