#include <avr/wdt.h>
#include <Servo.h>
Servo servo;

void setup() {
  Serial.begin(9600);
  wdt_enable(WDTO_8S);  // สั่งให้เริ่มจับเวลา 2 วินาที ถ้าเกินให้ Reset โปรแกรมใหม่
  Serial.println("Reset");
  servo.attach(9);
}
void loop() {
  //wdt_disable();
  servo.write(0);
  Serial.println("1");
  delay(5000);

  servo.write(60);
  wdt_reset();
  Serial.println("2");
  delay(5000);

  servo.write(120);
  wdt_reset();
  Serial.println("3");
  delay(5000);

  servo.write(180);
  wdt_reset();
  Serial.println("4");
  delay(5000);

  wdt_reset();
}