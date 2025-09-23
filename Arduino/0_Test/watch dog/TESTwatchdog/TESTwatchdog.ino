#include <avr/wdt.h>
void setup() {
  Serial.begin(9600);
  wdt_enable(WDTO_2S);
  Serial.println("reset");
}
void loop() {
  Serial.println("loop");
  delay(1000);
  wdt_reset();
  delay(1000);
  wdt_reset();
  delay(1000);
  wdt_reset();
}
