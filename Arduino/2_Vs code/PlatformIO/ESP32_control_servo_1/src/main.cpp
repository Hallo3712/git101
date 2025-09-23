#include <ESP32Servo.h>

Servo myservo;

int servoMin = 0;
int servoMax = 180;
int pos = 0;

void setup() {
  // ปรับ minPulse และ maxPulse ตามเซอร์โวที่คุณใช้ (ลองค่าต่าง ๆ เช่น 500-2400)
  myservo.attach(13, 400, 2500);
}

void loop() {
  for (pos = servoMin; pos <= servoMax; pos++) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = servoMax; pos >= servoMin; pos--) {
    myservo.write(pos);
    delay(15);
  }
}
