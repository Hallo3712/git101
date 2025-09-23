#include <ShiftRegister74HC595.h>
#include "input.h"

// create a global shift register object
// parameters: <number of shift registers>  sr(data pin, clock pin, latch pin)
ShiftRegister74HC595<1> sr(11, 12, 8);

#define led LED_BUILTIN

 
void setup() { 
  setup_sw();
  Serial.begin(9600);
  pinMode(led,OUTPUT);

  int dl1 = 50;
  digitalWrite(led,HIGH);
  delay(dl1);
  digitalWrite(led,LOW);
  delay(dl1);
  digitalWrite(led,HIGH);
  delay(dl1);
  digitalWrite(led,LOW);
  delay(dl1);
  digitalWrite(led,HIGH);
  delay(dl1);
  digitalWrite(led,LOW);
  delay(dl1);
  digitalWrite(led,HIGH);
  delay(dl1);
  digitalWrite(led,LOW);
  delay(dl1);
  digitalWrite(led,HIGH);
  delay(dl1);
  digitalWrite(led,LOW);
  delay(dl1);

  Serial.println("end setup");
  
}

void loop() {
  //get_value();
  //trick();

  sr.setAllHigh(); // set all pins HIGH
  Serial.println("1");
  digitalWrite(led,HIGH);
  delay(500);
  
  sr.setAllLow(); // set all pins LOW
  Serial.println("0");
  digitalWrite(led,LOW);
  delay(500);

}


void trick(){
  switch(count){
    case 1:
      sr.set(4, LOW); // set single pin HIGH
      sr.set(1, HIGH); // set single pin HIGH
      break;
    case 2:
      sr.set(1, LOW); // set single pin HIGH
      sr.set(2, HIGH); // set single pin HIGH
      break;
    case 3:
      sr.set(2, LOW); // set single pin HIGH
      sr.set(3, HIGH); // set single pin HIGH
      break;
    case 4:
      sr.set(3, LOW); // set single pin HIGH
      sr.set(4, HIGH); // set single pin HIGH
      break;

  }
}







