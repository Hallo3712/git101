#แสดงวันเวลาในกล้อง/วิดีโอ
import cv2
import datetime


cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\birdFlying.mp4")
while (cap.isOpened()):
       check , frame = cap.read() # อ่านภาพจากกล้อง

       if check == True: # ถ้าอ่านภาพได้
          
          current_time = str(datetime.datetime.now().strftime("%H:%M:%S")) # ดึงเวลาปัจจุบัน
          #current_time = str(datetime.datetime.now()) # ดึงเวลาปัจจุบัน
          
          cv2.putText(frame, "14_Time_Date_on_VDO", (10, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
          cv2.putText(frame, current_time, (10, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)


          cv2.imshow("Video", frame) # แสดงภาพจากกล้อง
          if cv2.waitKey(1) & 0xFF == ord("e"): # กดปุ่ม e เพื่อออกจากโปรแกรม 
                 break
       else:
          break
            
cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่าง