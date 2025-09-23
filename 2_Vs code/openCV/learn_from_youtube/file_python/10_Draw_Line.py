# วาด
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ

imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ

cv2.arrowedLine(imgresize, (200, 200), (600, 400), (255, 0, 255), 5) # วาดเส้น
# cv2.line(ภาพ, จุดเริ่มต้น, จุดสิ้นสุด, สี(BGR), ความหนา)

cv2.line(imgresize, (400, 100), (100, 300), (0, 255, 0), 3) # วาดเส้น

cv2.imshow('10_Draw_Line', imgresize) # แสดงภาพ

cv2.waitKey(0) # รอคีย์
cv2.destroyAllWindows() # ปิดหน้าต่าง
