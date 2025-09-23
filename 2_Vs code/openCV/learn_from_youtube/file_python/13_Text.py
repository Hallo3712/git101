# วาดข้อความ
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ

imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ

cv2.putText(imgresize, 'Hello OpenCV', (350, 250), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2) # วาดข้อความ
# cv2.putText(ภาพ, 'ข้อความ', (ตำแหน่ง x, ตำแหน่ง y), ฟอนต์, ขนาดฟอนต์, สี(BGR), ความหนา)
# เปลี่ยนฟอนต์ได้ เช่น cv2.FONT_HERSHEY_COMPLEX, cv2.FONT_HERSHEY_TRIPLEX, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX
# หาได้ที่ https://docs.opencv.org (docs)
# สามารถใส่ตัวเลขแทนได้ จะได้ไม่ต้องพิมพ์ยาว เช่น 0, 1, 2, 3, 4, 5

cv2.imshow('13_Text', imgresize) # แสดงภาพ
cv2.waitKey(0) # รอคีย์
cv2.destroyAllWindows() # ปิดหน้าต่าง