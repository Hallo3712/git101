#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define servosetzero 200
#define servoMin 320  // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax 860  // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 10

int dl_1 = 10;
int p0 = 0; // position for servo1

void setup_pc9685(){
  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
}

void servo_turn() {

  p0 = map(p0, 0, 180, servoMin, servoMax);

  pwm.setPWM(0, servosetzero, p0);
  Serial.println(p0);
  delay(dl_1);

}






