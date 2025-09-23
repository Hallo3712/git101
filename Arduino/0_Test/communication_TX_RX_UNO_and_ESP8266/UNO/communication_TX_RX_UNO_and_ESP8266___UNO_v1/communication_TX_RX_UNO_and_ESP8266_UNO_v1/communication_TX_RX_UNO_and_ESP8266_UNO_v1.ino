#include <SoftwareSerial.h>
SoftwareSerial espserial(2, 3);  // RX TX

#define led LED_BUILTIN
                   
int d1 = 101;
int d2 = 102;
int d3 = 103;
int d4 = 104;
int d5 = 105;

String status;

void setup() {
  Serial.begin(9600);
  espserial.begin(9600);
  pinMode(led, OUTPUT);

  Serial.println("end setup Arduino");
}

void loop() {
  status = String(d1) + "," + String(d2) + "," + String(d3) + "," + String(d4) + "," + String(d5);

  Serial.println(status);
  espserial.println(status);

  digitalWrite(led, !digitalRead(led));
  Serial.println("led is " + String(digitalRead(led)));
  delay(1000);
  
}
















