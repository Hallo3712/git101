# วาดวงกลม
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ

imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ


cv2.circle(imgresize, (350, 250), 100, (255, 0, 0), 5) # วาดวงกลม
# cv2.circle(ภาพ, จุดศูนย์กลาง, รัศมี, สี(BGR), ความหนา) ความหนา ใส่ -1 เพื่อเติมสี

cv2.imshow('11_Draw_rectangle', imgresize) # แสดงภาพ
cv2.waitKey(0) # รอคีย์
cv2.destroyAllWindows() # ปิดหน้าต่าง