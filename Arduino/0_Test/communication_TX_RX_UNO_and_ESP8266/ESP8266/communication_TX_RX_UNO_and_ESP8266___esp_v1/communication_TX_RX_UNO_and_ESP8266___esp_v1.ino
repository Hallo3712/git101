#include <SoftwareSerial.h>
SoftwareSerial espserial(D5, D6);  // RX TX

int d1;
int d2;
int d3;
int d4;
int d5;
String strIN;
bool mode;

int result;

void setup() {
  Serial.begin(9600);
  espserial.begin(9600);

  delay(3000);
  Serial.println("end setup esp");
  delay(3000);
}

void loop() {
  if (espserial.available() && mode == 1) {
    Serial.println("in espserial");
    strIN = espserial.readString();
    Serial.println(strIN);

    result = sscanf(strIN.c_str(),"%d,%d,%d,%d,%d",&d1,&d2,&d3,&d4,&d5);
    Serial.println("result is " + String(result));

    Serial.println("d1 is " + String(d1) + " d2 is " + String(d2) + " d3 is " + String(d3) + " d4 is " + String(d4) + " d5 is " + String(d5) );


    delay(100);
  }
  if (Serial.available()) {
    Serial.println("in serial");
    strIN = Serial.readString();
    Serial.println(strIN);
    mode = !mode;
    Serial.println("mode is " + String(mode));
    delay(100);
  }
}
