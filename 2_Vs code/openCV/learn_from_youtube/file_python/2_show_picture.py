# แสดงผลภาพ
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ
cv2.imshow("2_show_picture", img) # แสดงภาพ
cv2.waitKey(0) # รอการกดปุ่มใดๆ
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ