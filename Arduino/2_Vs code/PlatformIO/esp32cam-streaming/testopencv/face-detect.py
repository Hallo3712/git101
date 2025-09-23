import cv2 as cv

img = cv.imread(r"C:\Users\PC6958\Desktop\TAWAN\Arduino\vs code\testopencv\medium.jpg")
face_mode = cv.CascadeClassifier(cv.data.haarcascades + "haarcascade_frontalface_default.xml")
#img = cv.imread('medium.jpg')
#face_mode = cv.CascadeClassifier(cv.data.haarcascades + "haarcascade_frontalface_default.xml")


gray_scale = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
faces = face_mode.detectMultiScale(gray_scale)

for (x,y,w,h) in faces:
    cv.rectangle(img, (x,y) , (x+w,y+h) , (255,255,0) , 2)



cv.imshow('image',img)
cv.waitKey(0)
cv.destroyAllWindows()



