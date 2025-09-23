#include <Servo.h>
#include <avr/wdt.h>

Servo servo1, servo2;
int p0 = 0, p1 = 45, p2 = 90, p3 = 135, p4 = 180;
int w = 1000;
void setup() {
  Serial.begin(9600);
  servo1.attach(9);  //กำหนดขาของ servo1 เป็นขา 9 พัง
  servo1.attach(10);  //กำหนดขาของ servo2 เป็นขา 10 ใช้ได้
  wdt_enable(WDTO_8S);  // สั่งให้เริ่มจับเวลา 4 วินาที ถ้าเกินให้ Reset โปรแกรมใหม่
  Serial.println("Reset");
}

void loop() {
  servoTurn(p0, 2000);
  wdt_reset();

  servoTurn(p1, 2000);
  wdt_reset();

  servoTurn(p2, 2000);
  wdt_reset();

  servoTurn(p3, 2000);
  wdt_reset();

  servoTurn(p4, 2000);
  wdt_reset();
}

void servoTurn(int p, int w1) {
  servo1.write(p);
  delay(w1);
  Serial.print("servo1 write is ");
  Serial.println(p);

  int pForServo2 = p-10;
  servo2.write(pForServo2);
  delay(w1);
  Serial.print("servo2 write is ");
  Serial.println(pForServo2);
}




