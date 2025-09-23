# วาดกรอบสี่เหลี่ยม
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ

imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ


cv2.rectangle(imgresize, (100, 100), (400, 300), (0, 255, 0), 3) # วาดกรอบสี่เหลี่ยม 
# cv2.rectangle(ภาพ, จุดมุมบนซ้าย, จุดมุมล่างขวา, สี(BGR), ความหนา) ความหนา ใส่ -1 เพื่อเติมสี

cv2.imshow('11_Draw_rectangle', imgresize) # แสดงภาพ
cv2.waitKey(0) # รอคีย์
cv2.destroyAllWindows() # ปิดหน้าต่าง