#include <avr/wdt.h>

void setup() {
  Serial.begin(9600);
  wdt_enable(WDTO_3S);  // สั่งให้เริ่มจับเวลา 2 วินาที ถ้าเกินให้ Reset โปรแกรมใหม่
  Serial.println("Reset");
}
void loop() {
  //wdt_disable();
  Serial.println("1");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
  delay(2000);
  wdt_reset();
  Serial.println("2");
  delay(2000);
  wdt_reset();
  Serial.println("3");
  delay(2000);
  wdt_reset();
  Serial.println("4");
  delay(2000);
  wdt_reset();
}