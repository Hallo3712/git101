#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define servoMin 200  // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax 840  // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 3
#define delayWaiting 2000

void setpca9685() {
  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
  delay(20);
}

void servo_turn() {
  pwm.setPWM(0, servoMin, val1);
  pwm.setPWM(1, servoMin, val2);
  pwm.setPWM(2, servoMin, val3);
  pwm.setPWM(3, servoMin, val4);
  pwm.setPWM(4, servoMin, val5);
}
