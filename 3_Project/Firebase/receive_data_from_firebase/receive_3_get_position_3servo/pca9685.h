#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define servosetzero 200
#define servoMin 320  // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax 860  // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 10

int dl_1 = 10;

int p0 = 0;  // position for servo0
#define servo_0_setzero 200
#define servo_0_Min 320
#define servo_0_Max 860

int p1 = 0;  // position for servo1
#define servo_1_setzero 200
#define servo_1_Min 320
#define servo_1_Max 860

int p2 = 0;  // position for servo2
#define servo_2_setzero 200
#define servo_2_Min 320
#define servo_2_Max 860

void setup_pc9685() {
  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
}
/*
void servo_turn() {

  p0 = map(p0, 0, 180, servoMin, servoMax);

  pwm.setPWM(0, servosetzero, p0);
  Serial.println(p0);
  delay(dl_1);

}
*/

void servo_turn_to_find_min_max(int servo, int position) {  //   servo_turn(servo,position);

  position = map(position, 0, 180, servoMin, servoMax);

  pwm.setPWM(servo, servosetzero, position);
  Serial.println(position);
  delay(dl_1);
}

void servo_turn(int servo, int position) {
  switch (servo) {
    case 0:
      position = map(position, 0, 180, servo_0_Min, servo_0_Max);
      pwm.setPWM(servo, servo_0_setzero, position);
      Serial.println(position);
      break;
    case 1:
      position = map(position, 0, 180, servo_1_Min, servo_1_Max);
      pwm.setPWM(servo, servo_1_setzero, position);
      Serial.println(position);
      break;
    case 2:
      position = map(position, 0, 180, servo_2_Min, servo_2_Max);
      pwm.setPWM(servo, servo_2_setzero, position);
      Serial.println(position);
      break;
  }
  delay(dl_1);
}
