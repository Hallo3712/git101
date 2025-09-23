#define S0_PIN D1  // กำหนดพินควบคุม S0
#define S1_PIN D2  // กำหนดพินควบคุม S1
#define S2_PIN D3  // กำหนดพินควบคุม S2
#define S3_PIN D4  // กำหนดพินควบคุม S3

#define Z_PIN A0  // พินอ่านสัญญาณอนาล็อก (แรงดัน 0-1V)

int volthigh = 0;
int voltlow = 0;
int val1 = 0;
int val2 = 0;
int val3 = 0;

void setup() {
  Serial.begin(115200);  // ตั้งค่า Serial Communication
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(Z_PIN, INPUT);

  Serial.println("ESP8266 Setup Complete");
}

void loop() {
  int dl1 = 10;

  // อ่านค่าจากช่อง Y0, Y1, Y2
  val1 = analog_Read(0);  // อ่านค่าจาก Y0
  val2 = analog_Read(1);  // อ่านค่าจาก Y1
  val3 = analog_Read(2);  // อ่านค่าจาก Y2

  volthigh = analog_Read(14);
  voltlow  = analog_Read(15);
  //Serial.print(" volthigh = " + String(volthigh));
  //Serial.println(" voltlow = " + String(voltlow));

  // แสดงผลค่าที่อ่านได้
  //Serial.print("val1 = " + String(val1));
  //Serial.print(" val2 = " + String(val2));
  //Serial.println(" val3 = " + String(val3));

  // แปลงค่าจาก 0-1023 ให้เป็น 0-180
  val1 = map(val1, voltlow, volthigh, 0, 210);
  val2 = map(val2, voltlow, volthigh, 0, 210);
  val3 = map(val3, voltlow, volthigh, 0, 210);

  // แสดงผลค่าที่อ่านได้
  Serial.print("val1 = " + String(val1));
  Serial.print(" val2 = " + String(val2));
  Serial.println(" val3 = " + String(val3));

  delay(1000);  // หน่วงเวลา 1 วินาที
}

// ฟังก์ชันอ่านค่าอนาล็อกจาก Multiplexer
int analog_Read(byte ch) {
  boolean s0[16] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
  boolean s1[16] = { 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 };
  boolean s2[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 };
  boolean s3[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };

  // เขียนค่าควบคุมไปที่พิน S0-S3
  digitalWrite(S0_PIN, s0[ch]);
  digitalWrite(S1_PIN, s1[ch]);
  digitalWrite(S2_PIN, s2[ch]);
  digitalWrite(S3_PIN, s3[ch]);

  delay(1);                  // หน่วงเวลาเล็กน้อย
  return analogRead(Z_PIN);  // อ่านค่าจากพินอนาล็อก
}
