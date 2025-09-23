// MOTOR & STEPPER MOTOR Series :: http://fitrox.lnwshop.com
// EP.10 PCA9685 Fade LED


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
// Default Address ไม่ต้องกำหนดใดๆ เนื่องจาก Library กำหนดไว้ให้แล้ว   
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int pwmLevel = 0;
int fadeAmount = 25;

void setup() {
  Serial.begin(9600);
  // เริ่มใช้งาน PWM
  pwm.begin();

  // Set PWM Frequency = 1000 Hz
  pwm.setPWMFreq(1000);
  Serial.println("PCA9685 Fading LED");
  delay(1000);
}

void loop() {
  // ส่งค่า PWM ออก
  pwm.setPWM(0, 0, pwmLevel);
  Serial.println(pwmLevel);

  // เพิ่มค่า PWM
  pwmLevel = pwmLevel + fadeAmount;

  // หากค่าอยู่นอกช่วงให้กลับค่า
  if(pwmLevel <= 0 || pwmLevel >= 4095){
    fadeAmount = -fadeAmount;
  }

  delay(30);
}