int S1 = 8;
int S2 = 9;
int S3 = 10;
int S4 = 11;
int data_in = A0;  // ใช้ A0 แทนการอ่านค่าผ่านขาอนาล็อก

void setup() {
  // เริ่มต้นการสื่อสารผ่านพอร์ตอนุกรมที่ 9600 บิตต่อวินาที
  Serial.begin(9600);
  
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);

  pinMode(data_in, INPUT);
}

void loop() {
  String output = "";  // สตริงสำหรับเก็บผลลัพธ์ทั้งหมด

  // เลือกช่อง 0 ถึง 2
  for (int i = 0; i < 3; i++) {
    // ใช้การ shift เพื่อเลือกขา S1-S4
    digitalWrite(S1, (i & 1)); 
    digitalWrite(S2, (i & 2) >> 1);
    digitalWrite(S3, (i & 4) >> 2);
    digitalWrite(S4, (i & 8) >> 3);

    // อ่านค่าจาก data_in (A0)
    int value = analogRead(data_in);

    // แปลงค่าจาก 0-1023 ไปเป็น 0-180
    int mappedValue = map(value, 0, 1023, 0, 180);

    // สร้างข้อความที่เก็บในตัวแปร output
    output += "Channel_" + String(i) + " : " + String(mappedValue);  
    
    // เพิ่มเครื่องหมายคอมมาและช่องว่างหากยังไม่ถึงช่องสุดท้าย
    if (i < 2) {
      output += " , ";
    }
  }

  // ส่งผลลัพธ์ทั้งหมดไปยัง Serial Monitor หรือ Serial Plotter
  Serial.println(output);

  delay(500);  // รอ 500ms ก่อนที่จะเลือกช่องถัดไป
}
