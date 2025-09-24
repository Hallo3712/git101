import cv2

# โหลดภาพจากไฟล์
# ตรวจสอบให้แน่ใจว่าไฟล์ภาพอยู่ในโฟลเดอร์เดียวกัน
image = cv2.imread("D:/Pictures/ant.jpg")

# ตรวจสอบว่าภาพถูกโหลดมาได้หรือไม่
if image is None:
    print("Error: Could not load the image.")
else:
    # แสดงภาพในหน้าต่างชื่อ 'Original Image'
    cv2.imshow('Original Image', image)

    # รอให้มีการกดปุ่มใดๆ เพื่อปิดหน้าต่าง
    cv2.waitKey(0)

    # ปิดหน้าต่างทั้งหมดของ OpenCV
    cv2.destroyAllWindows()