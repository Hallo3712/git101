# Motion Tracker
import cv2
cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\walking2.mp4")
check , frame1 = cap.read() 
check , frame2 = cap.read() 
while (cap.isOpened()): 
    if check == True :
        motiondiff = cv2.absdiff(frame1, frame2) # หาความแตกต่างระหว่าง frame1 กับ frame2
        gray = cv2.cvtColor(motiondiff, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray, (5, 5), 0) # ลด noise ด้วย GaussianBlur
        thresh,result = cv2.threshold(blur, 15, 255, cv2.THRESH_BINARY)
        dilation = cv2.dilate(result,None,iterations=3) # ขยายขอบเขตของวัตถุที่เคลื่อนไหวให้ใหญ่ขึ้น
        contours, hierarchy = cv2.findContours(dilation, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

        # วาด 4 เหลี่ยม ในวัตถุที่กำลังเคลื่อนที่
        for contour in contours:
            (x,y,w,h) = cv2.boundingRect(contour)
            if cv2.contourArea(contour)<2500:
                continue 
            cv2.rectangle(frame1,(x,y),(x+w,y+h),(0,255,0),2)

        cv2.imshow("vdo", frame1) 
        frame1 = frame2
        check, frame2 = cap.read()
        if cv2.waitKey(20) != -1: # กดปุ่มใดๆ เพื่อออกจากโปรแกรม
            break
    else:
        break

cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ
