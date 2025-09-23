// MOTOR & STEPPER MOTOR Series :: http://fitrox.lnwshop.com
// EP.10 PCA9685 4Step LED


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
// Default Address ไม่ต้องกำหนดใดๆ เนื่องจาก Library กำหนดไว้ให้แล้ว   
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

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
// write ค่า PWM 25%
  Serial.println("PWM 25%");
  pwm.setPWM(0, 0, 1024);
  delay(3000);
// write ค่า PWM 50%
  Serial.println("PWM 50%");
  pwm.setPWM(0, 0, 2048);
  delay(3000);
// write ค่า PWM 75%
  Serial.println("PWM 75%");
  pwm.setPWM(0, 0, 3072);
  delay(3000);
// write ค่า PWM 100%
  Serial.println("100%");
  pwm.setPWM(0, 0, 4095);
  delay(3000);
}