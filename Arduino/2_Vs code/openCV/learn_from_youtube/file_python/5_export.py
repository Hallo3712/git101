# การ export ภาพ    
import cv2

img = cv2.imread("image/cat.jpg",0) # อ่านภาพ
imgresize = cv2.resize(img, (700, 500)) # ปรับขนาดภาพ
cv2.imshow("output", imgresize) # แสดงภาพ

cv2.imwrite("image/output.jpg", imgresize) # export ภาพ

 
cv2.waitKey(0) # รอการกดปุ่มใดๆ
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ