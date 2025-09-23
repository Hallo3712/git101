# อ่านภาพให้เป็น determinance ดูว่าภาพมีกี่มิติ ดูว่า img.ndim เป็นตัวแปรชนิดไหน
import cv2

img = cv2.imread("image/cat.jpg") # อ่านภาพ
print(type (img.ndim))
print(img.ndim)
print(img)





