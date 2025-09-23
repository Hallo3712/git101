#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);  // TX, RX อันนี้ถูกแล้ว (TX, RX) ต่อเข้ากับตัวต้านทาน 2k และ 1k

int relay=5;

void setup() {
  Serial.begin(9600);  // ถ้าจะเข้า AT comand ให้ใช้ 38400 แต่ถ้าจะ รับ-ส่ง ข้อมูลให้ใช้ 9600
  while (!Serial)
    ;
  mySerial.begin(9600);
  pinMode(relay,OUTPUT);
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());

    /*if (mySerial.read() == 'A') {
      //digitalWrite(relay,!digitalRead(relay));
       mySerial.write("A");
    }*/
  }
}