#include <ShiftRegister74HC595.h>

// create a global shift register object
// parameters: <number of shift registers>  sr(data pin, clock pin, latch pin)
//ShiftRegister74HC595<1> sr(D1, D2, D3);
ShiftRegister74HC595<1> sr(11, 12, 8);



void trick() {
  Serial.println("trick");
  switch (count) {
    case 1:
      sr.set(4, LOW);   // set single pin HIGH
      sr.set(1, HIGH);  // set single pin HIGH
      break;
    case 2:
      sr.set(1, LOW);   // set single pin HIGH
      sr.set(2, HIGH);  // set single pin HIGH
      break;
    case 3:
      sr.set(2, LOW);   // set single pin HIGH
      sr.set(3, HIGH);  // set single pin HIGH
      break;
    case 4:
      sr.set(3, LOW);   // set single pin HIGH
      sr.set(4, HIGH);  // set single pin HIGH
      break;
  }
}

void all_high_all_low_74hc595() {
  Serial.println("all_high_all_low_74hc595");
  sr.setAllHigh();  // set all pins HIGH
  Serial.println("1");
  digitalWrite(led, led_high);
  delay(500);

  sr.setAllLow();  // set all pins LOW
  Serial.println("0");
  digitalWrite(led, led_low);
  delay(500);
}
