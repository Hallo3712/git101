#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // TX, RX อันนี้ถูกแล้ว (TX, RX) ต่อเข้ากับตัวต้านทาน 2k และ 1k
void setup()
{
  Serial.begin(38400); // ถ้าจะเข้า AT comand ให้ใช้ 38400 แต่ถ้าจะ รับ-ส่ง ข้อมูลให้ใช้ 9600
  //ถ้าจะเข้า AT command ต้องตั้ง  Both NL & CR ด้วย
  //ใช้ baud rate ที่ 38400 baud
  //1 ถอด vcc ของ hc05 
  //2 กดปุ่มค้างไว้
  //3 เสียบ vcc
  while (!Serial);
  mySerial.begin(38400);
}
void loop()
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}