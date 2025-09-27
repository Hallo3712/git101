import cv2
from ultralytics import YOLO


lane_points = []
test_mode = False # เพิ่มตัวแปรสถานะสำหรับโหมดทดสอบ

# Mouse callback function เพื่อรับพิกัดและคำนวณ slope
def mouse_callback(event, x, y, flags, param):
    global lane_points, test_mode
    
    if test_mode: # ถ้าอยู่ในโหมดทดสอบ
        if event == cv2.EVENT_LBUTTONDOWN:
            orig_x = int(x / ratio)
            orig_y = int(y / ratio)
            print(f"Test mode clicked at: X={orig_x}, Y={orig_y}")
            
            # ใช้เงื่อนไขเดียวกันกับบรรทัดที่ 135-140
            divider_x_at_vehicle = get_lane_divider_x(orig_y)
            
            # ตรวจสอบว่าคลิกอยู่ในพื้นที่ใด
            if orig_x > divider_x_at_vehicle and orig_y < line_y_in:
                print("-> เลนขวา(ขาเข้า)(ก่อนผ่านเส้น-ยังไม่นับ)")
            elif orig_x < divider_x_at_vehicle and orig_y > line_y_out:
                print("-> เลนซ้าย(ขาออก)(ก่อนผ่านเส้น-ยังไม่นับ)")
            elif orig_x > divider_x_at_vehicle and orig_y >= line_y_in:
                print("-> เลนขวา(ขาเข้า)(หลังจากผ่านเส้น-นับแล้ว)")
            elif orig_x < divider_x_at_vehicle and orig_y <= line_y_out:
                print("-> เลนซ้าย(ขาออก)(หลังจากผ่านเส้น-นับแล้ว)")
            else:
                print("-> อยู่นอกพื้นที่สำหรับนับค่า")

    else: # โหมดปกติ (สำหรับการคำนวณ slope)
        if event == cv2.EVENT_LBUTTONDOWN:
            orig_x = int(x / ratio)
            orig_y = int(y / ratio)
            print(f"Mouse clicked at: X={orig_x}, Y={orig_y}")
            lane_points.append((orig_x, orig_y))

            if len(lane_points) == 2:
                x1, y1 = lane_points[0]
                x2, y2 = lane_points[1]

                if y2 != y1: 
                    slope = (x2 - x1) / (y2 - y1)
                    intercept = x2 - slope * y2
                    print(f"\nLane divider calculated:")
                    print(f"slope = {slope:.3f}")
                    print(f"intercept = {intercept:.1f}")
                    print(f"Equation: x = {slope:.3f} * y + {intercept:.1f}")

                lane_points.clear()


def get_lane_divider_x(y):
    """Calculate x coordinate of lane divider at given y coordinate"""
    return int(lane_divider_slope * y + lane_divider_intercept)

def draw_sloped_lane_divider(frame, y_start, y_end):
    """Draw sloped lane divider based on curb"""
    x_start = get_lane_divider_x(y_start)
    x_end = get_lane_divider_x(y_end)
    cv2.line(frame, (x_start, y_start), (x_end, y_end), (255, 0, 255), 3)


# ตั้งค่าตัวแปรต่างๆ
ratio = 0.7
line_y_out = 200 
line_y_in = 250 
lane_divider_slope = -0.921 
lane_divider_intercept = 500.4
divider_x_at_out = get_lane_divider_x(line_y_out)
divider_x_at_in = get_lane_divider_x(line_y_in)
name = "YOLO car count"

class_count_in = {}
class_count_out = {}
crossed_in_ids = set()
crossed_out_ids = set()

# โหลด YOLO model
model = YOLO('yolov8n.pt')
class_list = model.names
# print(class_list)


cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\car.mp4")
cv2.namedWindow(name)
cv2.setMouseCallback(name,mouse_callback)

while (cap.isOpened()): 
    ret , frame = cap.read()
    
    shape = frame.shape
    new_width = int(frame.shape[1] * ratio)
    new_height = int(frame.shape[0] * ratio)

    # print(f"after resize is ({new_height}, {new_width})")

    if not ret:
        print("vdo false")
        break
    # print("line_y_out "+str(line_y_out))
    
    draw_sloped_lane_divider(frame,y_start=140,y_end=frame.shape[0])

    cv2.line(frame,(0,line_y_out),(divider_x_at_out,line_y_out),(0,0,255),3)
    cv2.putText(frame,"Befor IN",(700,line_y_in-10),cv2.FONT_HERSHEY_DUPLEX,1,(255,0,0),3)
    cv2.putText(frame,"IN",(800,line_y_in+40),cv2.FONT_HERSHEY_DUPLEX,1,(255,0,0),3)
    cv2.line(frame,(divider_x_at_in,line_y_in),(frame.shape[1],line_y_in),(255,0,0),3)
    cv2.putText(frame,"OUT",(10,line_y_out-10),cv2.FONT_HERSHEY_DUPLEX,1,(0,0,255),3)
    cv2.putText(frame,"Befor OUT",(10,line_y_out+50),cv2.FONT_HERSHEY_DUPLEX,1,(0,0,255),3)

    if not test_mode: # เมื่อไม่ได้อยู่ในโหมดทดสอบ จึงจะทำการตรวจจับรถ
        results = model.track(frame,persist=True,classes=[0,1,2,3,5,7],device='cpu',verbose=False)

        if results[0].boxes.data is not None:
            boxes = results[0].boxes.xyxy.cpu()
            track_ids = results[0].boxes.id.int().cpu().tolist()
            class_indices = results[0].boxes.cls.int().cpu().tolist()
            confidences = results[0].boxes.conf.cpu()

            for box,track_id,class_idx,conf in zip(boxes,track_ids,class_indices, confidences):
                print(box,track_id,class_idx,conf)
                x1 ,y1 ,x2 ,y2 = map (int,box)

                cx = (x1+x2)//2
                cy = (y1+y2)//2

                class_name = class_list[class_idx]
                divider_x_at_vehicle = get_lane_divider_x(cy)

                cv2.circle(frame,(cx,cy),4,(0,0,255),5)
                cv2.putText(frame,f"ID: {track_id} {class_name} ", (x1,y1 - 10),cv2.FONT_HERSHEY_DUPLEX,0.6,(0,255,0),2)
                
                cv2.rectangle(frame, (x1,y1),(x2,y2),(0,255,0),2)

                if cx > divider_x_at_vehicle and cy < line_y_in and track_id not in crossed_in_ids:
                    crossed_in_ids.add(track_id)
                    class_count_in[class_name] = class_count_in.get(class_name, 0) + 1
                if cx < divider_x_at_vehicle and cy > line_y_out and track_id not in crossed_out_ids:
                    crossed_out_ids.add(track_id)
                    class_count_out[class_name] = class_count_out.get(class_name, 0) + 1
                # print("divider_x_at_vehicle "+str(divider_x_at_vehicle))
                # if class_name == 'car' and track_id == 46:
                #     print(f"DEBUG OUT: cx={cx}, cy={cy}, divider_x={divider_x_at_vehicle}, line_y_out={line_y_out}")
                

    scaled_frame = cv2.resize(frame, (new_width, new_height))
    
    # แสดงสถานะโหมดทดสอบ
    mode_text = "Test Mode" if test_mode else "Normal Mode"
    cv2.putText(scaled_frame, mode_text, (10, 30), cv2.FONT_HERSHEY_DUPLEX, 1, (0, 255, 255), 2)
    
    cv2.imshow(name,scaled_frame)
    key = cv2.waitKey(1) & 0xff # แก้ไข waitKey ให้รับคีย์ในลูปต่อเนื่อง
    
    if key == ord('q'):
        print("button q pressed")
        break
    elif key == ord('t'): # เพิ่มเงื่อนไขเพื่อสลับโหมด
        test_mode = not test_mode
        print(f"Test Mode is now {'ON' if test_mode else 'OFF'}")


cap.release()
cv2.destroyAllWindows()