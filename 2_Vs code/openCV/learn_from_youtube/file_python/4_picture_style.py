# กำหนดรูปแบบของภาพ 
# cv2.imread("image/cat.jpg" , cv2.IMREAD_COLOR)  อ่านภาพสี
# cv2.imread("image/cat.jpg" , cv2.IMREAD_GRAYSCALE)  อ่านภาพขาวดำ
# cv2.imread("image/cat.jpg" , cv2.IMREAD_UNCHANGED)  อ่านภาพแบบเดิม  ไม่เปลี่ยนแปลง
# cv2.imread("image/cat.jpg" , 0)  อ่านภาพขาวดำ
# cv2.imread("image/cat.jpg" , 1)  อ่านภาพสี
# cv2.imread("image/cat.jpg" , -1)  อ่านภาพแบบเดิม  ไม่เปลี่ยนแปลง

import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ
imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ

cv2.imshow("output", imgresize) # แสดงภาพ
cv2.waitKey(0) # รอการกดปุ่มใดๆ
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ