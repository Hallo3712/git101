# อ่านภาพ
# ปรับขนาดภาพ
# แสดงผลภาพ
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ
imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ

cv2.imshow("3_resize", imgresize) # แสดงภาพ
cv2.waitKey(0) # รอการกดปุ่มใดๆ
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ