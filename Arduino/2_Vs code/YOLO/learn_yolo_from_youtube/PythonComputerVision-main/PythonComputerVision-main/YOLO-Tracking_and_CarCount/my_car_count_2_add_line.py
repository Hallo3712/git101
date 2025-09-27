import cv2
from ultralytics import YOLO


lane_points = []
# Mouse callback function เพื่อรับพิกัดและคำนวณ slope
def mouse_callback(event, x, y, flags, param):
    global lane_points
    if event == cv2.EVENT_LBUTTONDOWN:
        orig_x = int(x / ratio)
        orig_y = int(y / ratio)
        print(f"Mouse clicked at: X={orig_x}, Y={orig_y}")
        lane_points.append((orig_x, orig_y))

        if len(lane_points) == 2:
            # คำนวณ slope และ intercept จากสองจุด
            x1, y1 = lane_points[0]
            x2, y2 = lane_points[1]

            if y2 != y1:  # ป้องกันการหารด้วยศูนย์
                slope = (x2 - x1) / (y2 - y1)
                intercept = x2 - slope * y2
                print(f"\nLane divider calculated:")
                print(f"slope = {slope:.3f}")
                print(f"intercept = {intercept:.1f}")
                print(f"Equation: x = {slope:.3f} * y + {intercept:.1f}")

            lane_points.clear()  # รีเซ็ตสำหรับการคำนวณครั้งถัดไป

def get_lane_divider_x(y):
    """Calculate x coordinate of lane divider at given y coordinate"""
    return int(lane_divider_slope * y + lane_divider_intercept)

def draw_sloped_lane_divider(frame, y_start, y_end):
    """Draw sloped lane divider based on curb"""
    x_start = get_lane_divider_x(y_start)
    x_end = get_lane_divider_x(y_end)
    cv2.line(frame, (x_start, y_start), (x_end, y_end), (255, 0, 255), 3)


# ตั้งค่าตัวแปรต่างๆ
ratio = 0.7  # สเกลของรูปภาพที่แสดง (กำหนดเอง)
line_y_out = 225   # ค่าแกน y (ฝั่งขาออก, ซ้าย) (กำหนดเอง)
line_y_in = 300      # ค่าแกน y (ฝั่งขาเข้า, ขวา) (กำหนดเอง)
lane_divider_slope = -0.921   # คำนวนจากสูตรเส้นตรง
lane_divider_intercept = 500.4  # คำนวนจากสูตรเส้นตรง
divider_x_at_out = get_lane_divider_x(line_y_out)
divider_x_at_in = get_lane_divider_x(line_y_in)
name = "YOLO car count"

class_count_in = {}
class_count_out = {}
crossed_in_ids  = set()
crossed_out_ids = set()

# โหลด YOLO model
model = YOLO('yolov8n.pt')
class_list = model.names
# print(class_list)


cap = cv2.VideoCapture("C:\\Users\\PC6958\\Videos\\car.mp4")
cv2.namedWindow(name)
cv2.setMouseCallback(name,mouse_callback)

while (cap.isOpened()): 
    ret , frame = cap.read() # อ่านภาพจากกล้อง 
    
    shape = frame.shape
    # print("befor resize is "+str(shape))

    new_width = int(frame.shape[1] * ratio)
    new_height = int(frame.shape[0] * ratio)

    # print("after resize is ("+str(new_height)+", "+str(new_width)+ ")")
    # print(f"after resize is ({new_height}, {new_width})")
    #print("after resize is "+str(shape))

    if not ret:
        print("vdo false")
        break
    # print("line_y_out "+str(line_y_out))
    """

    print("line_y_in")
    print(line_y_in)

    print("divider_x_at_out")
    print(divider_x_at_out)

    print("divider_x_at_in")
    print(divider_x_at_in)
"""
    draw_sloped_lane_divider(frame,y_start=140,y_end=frame.shape[0])
    # cv2.arrowedLine(frame, (0, line_y_in), (950, line_y_out), (0, 0, 0), 3)
    # cv2.arrowedLine(frame, (divider_x_at_in,line_y_in), (divider_x_at_out,line_y_out), (255, 0, 0), 3)

    cv2.line(frame,(0,line_y_out),(divider_x_at_out,line_y_out),(0,0,255),3)
    cv2.line(frame,(divider_x_at_in,line_y_in),(frame.shape[1],line_y_in),(255,0,0),3)
    # cv2.putText(frame,"Befor IN",(700,line_y_in-10),cv2.FONT_HERSHEY_DUPLEX,1,(255,0,0),3)
    # cv2.putText(frame,"IN",(800,line_y_in+40),cv2.FONT_HERSHEY_DUPLEX,1,(255,0,0),3)
    # cv2.putText(frame,"OUT",(10,line_y_out-10),cv2.FONT_HERSHEY_DUPLEX,1,(0,0,255),3)
    # cv2.putText(frame,"Befor OUT",(10,line_y_out+50),cv2.FONT_HERSHEY_DUPLEX,1,(0,0,255),3)

    
    results = model.track(frame,persist=True,classes=[0,1,2,3,5,7],device='cpu',verbose=False)
    # print("result is "+str(results))

    if results[0].boxes.data is not None:
        # เก็บค่าที่ต้องการใช้
        boxes = results[0].boxes.xyxy.cpu()
        track_ids = results[0].boxes.id.int().cpu().tolist()
        class_indices = results[0].boxes.cls.int().cpu().tolist()
        confidences = results[0].boxes.conf.cpu()

        for box,track_id,class_idx,conf in zip(boxes,track_ids,class_indices, confidences):
            print(box,track_id,class_idx,conf)
            x1 ,y1 ,x2 ,y2 = map (int,box)

            cx = (x1+x2)//2
            cy = (y1+y2)//2
            # print("cx,cy is ")
            # print(cx,cy)

            class_name = class_list[class_idx]

            divider_x_at_vehicle = get_lane_divider_x(cy)

            cv2.circle(frame,(cx,cy),4,(0,0,255),5)
            cv2.putText(frame,f"ID: {track_id} {class_name} ", (x1,y1 - 10),cv2.FONT_HERSHEY_DUPLEX,0.6,(255,0,0),2)
            
        
            cv2.rectangle(frame, (x1,y1),(x2,y2),(0,255,0),2)

            if cx > divider_x_at_vehicle and cy < line_y_in and track_id not in crossed_in_ids:
                crossed_in_ids.add(track_id)
                class_count_in[class_name] = class_count_in.get(class_name, 0) + 1
            if cx < divider_x_at_vehicle and cy > line_y_out and track_id not in crossed_out_ids:
                crossed_out_ids.add(track_id)
                class_count_out[class_name] = class_count_out.get(class_name, 0) + 1
            # print("divider_x_at_vehicle "+str(divider_x_at_vehicle))
            print("class_count_out"+str(class_count_out))
            # print(class_count_out)
            print("class_count_in"+str(class_count_in))
            # print(class_count_in)

    # แสดงจำนวนบนรูป
    y_offset = 30
    cv2.putText(frame, "VEHICLES IN (Right Lane):", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0), 2)
    y_offset += 25
    
    for class_name, count in class_count_in.items():
        cv2.putText(frame, f"{class_name}: {count}", (30, y_offset),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
        y_offset += 25
        
    y_offset += 10
    cv2.putText(frame, "VEHICLES OUT (Left Lane):", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0), 2)
    y_offset += 25
    
    for class_name, count in class_count_out.items():
        cv2.putText(frame, f"{class_name}: {count}", (30, y_offset),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
        y_offset += 25

    scaled_frame = cv2.resize(frame, (new_width, new_height))
    cv2.imshow(name,scaled_frame)
    if cv2.waitKey(1) & 0xff == ord('q'):
        print("button q pressed")
        break

cap.release() # ปิดกล้อง
cv2.destroyAllWindows() # ปิดหน้าต่างแสดงภาพ
