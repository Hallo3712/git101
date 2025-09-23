// MOTOR & STEPPER MOTOR Series :: http://fitrox.lnwshop.com
// EP.10 PCA9685 Servo


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// สร้าง Object สำหรับโมดูล PCA9685 ให้ชื่อ pwm ค่า Address คือ Default (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define servosetzero 200
#define servoMin 334  //320//    // Pulse น้อยสุด กำหนด 150 จาก 4096
#define servoMax 840  //860//    // Pulse มากสุด กำหนด 1000 จาก 4096
#define servoStep 10

int dl_1 = 100;

int vr1 = A0;
int val;
int inlow = 10;
int inhigh = 540;

String data = "";

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนด Frequency = 60 Hz (เซอร์โวส่วนใหญ่จะทำงานที่ 50-60 Hz)
  delay(20);
  Serial.println("done setup");

  pinMode(vr1, INPUT);
}

void loop() {

  val = analogRead(vr1);
  Serial.println("read val is " + String(val));
  val = map(val, inlow, inhigh, 0, 180);
  Serial.println("val aft map is " + String(val));
  delay(100);


  val = map(val, 0, 180, servoMin, servoMax);

  pwm.setPWM(0, servosetzero, val);
  //Serial.println("write : " + String(val));
  delay(dl_1);


  if (Serial.available()) {
    data = Serial.readString();
    Serial.println("data is " + String(data));

    if (data == "hi\n") {
      Serial.println("get hi");
    } else if (data == "change\n") {
      Serial.println("change what (inlow,inhigh)");
      delay(100);
      while (1) {
        if (Serial.available()) {
          data = Serial.readString();
          Serial.println("if available is " + String(data));

          if (data == "break\n") {
            Serial.println("break");
            delay(1000);
            break;
          } else if (data == "inlow\n") {
            Serial.println("change inlow to?");
            delay(1000);
            while (1) {
              if (Serial.available()) {
                data = Serial.readString();
                inlow = data.toInt();
                Serial.println("inlow is " + String(inlow));
                delay(1000);
                break;
              }
            }
          } else if (data == "inhigh\n") {
            Serial.println("change inhigh to?");
            delay(1000);
            while (1) {
              if (Serial.available()) {
                data = Serial.readString();
                inhigh = data.toInt();
                Serial.println("inhigh is " + String(inhigh));
                delay(1000);
                break;
              }
            }
          }
        }
      }
    }
  }
}
