#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "var.h"
#include "var_servo.h"

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define servoMin 200  // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax 650  // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 3
#define delayWaiting 2000

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
  delay(20);

  setup_vr();
}

void loop() {
/*
  pwm.setPWM(0, 0, servoMax);
  delay(delayWaiting);
  pwm.setPWM(0, 0, servoMin);
  delay(delayWaiting);
*/
  val_vr1 = analogRead(vr1);
  Serial.println("val vr1 is " + String(val_vr1));

  p_servo1 = map(val_vr1,0,1023,servoMin,servoMax);
  Serial.println("p_servo1 is " + String(p_servo1));
  pwm.setPWM(0,servoMin,p_servo1);
  delay(30);




}













