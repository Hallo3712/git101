int analogPin = 12;  // ใช้ GPIO12 สำหรับการอ่านค่า analog (หรือใช้ GPIO13 ก็ได้)
int digitalPin = 13; // ใช้ GPIO32 สำหรับการอ่านค่า digital
int sensorValue = 0;

void setup() {
  Serial.begin(115200);  // เริ่มต้น Serial Monitor
  pinMode(digitalPin, INPUT); // กำหนด GPIO32 เป็น Input
  pinMode(analogPin, INPUT);  // กำหนด GPIO12 (หรือ GPIO13) เป็น Input
}

void loop() {
  sensorValue = analogRead(analogPin);  // อ่านค่าจาก A0 (ผ่าน GPIO12 หรือ GPIO13)
  int digitalValue = digitalRead(digitalPin);  // อ่านค่าจาก D0 (ผ่าน GPIO32)

  Serial.print("Analog Value: ");
  Serial.print(sensorValue);  // แสดงค่า analog
  Serial.print("  Digital Value: ");
  Serial.println(digitalValue);  // แสดงค่า digital

  delay(1000);  // หน่วงเวลา 1 วินาที
}
