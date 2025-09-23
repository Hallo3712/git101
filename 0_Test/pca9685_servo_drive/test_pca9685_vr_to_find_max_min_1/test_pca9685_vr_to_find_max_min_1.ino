// MOTOR & STEPPER MOTOR Series :: http://fitrox.lnwshop.com
// EP.10 PCA9685 Servo


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define servosetzero 200
#define servoMin   334//320//    // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax   840//860//    // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 10

int dl_1 = 100;

int vr1 = A0;
int val;

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
  delay(20);
  Serial.println("done setup");

  pinMode(vr1,INPUT);


}

void loop() {

  val = analogRead(vr1);
  val = map(val,0,1023,servoMin,servoMax);

  pwm.setPWM(0, servosetzero, val);
  Serial.println(val);
  delay(dl_1);
}