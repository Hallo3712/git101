// MOTOR & STEPPER MOTOR Series :: http://fitrox.lnwshop.com
// EP.10 PCA9685 Servo


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define servosetzero  200
#define servoMin 320  // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax 860  // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 10

int dl_1 = 100;

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
  delay(20);  
  Serial.println("done setup");
}

void loop() {
  // สร้าง Pulse ตั้งแต่ Min ไปหา Max แขนเซอร์โวจะค่อยๆ หมุนจาก 0 ไปจนถึง 180 องศา
  for (int pulse = servoMin; pulse < servoMax; pulse+=servoStep) {
    pwm.setPWM(0, servosetzero, pulse);
    Serial.println(pulse);
    delay(dl_1);
  }

  // เมื่อแขนหมุนไปสุดให้ค่อยๆ ลดจาก Max ไปจน Min แขนจะหมุนกลับจาก 180 ลงมาจน 0 องศา
  for (int pulse = servoMax; pulse > servoMin; pulse-=servoStep) {
    pwm.setPWM(0, servosetzero, pulse);
    Serial.println(pulse);
    delay(dl_1);
  }
}