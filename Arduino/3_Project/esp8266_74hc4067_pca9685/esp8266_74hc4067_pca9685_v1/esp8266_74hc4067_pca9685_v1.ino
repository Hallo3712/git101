#include "variable.h"
#include "function.h"
#include "pca9685.h"
#include "74hc4067.h"

int val[16];  // ตัวแปรจริง อยู่ที่นี่


void setup() {
  Serial.begin(115200);  // ตั้งค่า Serial Communication

  setup_74hc4067();
  setpca9685();

  Serial.println("ESP8266 Setup Complete");
}

void loop() {
  int dl1 = 10;

  read_all_channel_74hc4067();

  Serial.print(" volthigh = " + String(volthigh));
  Serial.println(" voltlow = " + String(voltlow));

  //แสดงผลค่าที่อ่านได้
  /*
  
  Serial.print("val1 = " + String(val1));
  Serial.print(" val2 = " + String(val2));
  Serial.print(" val3 = " + String(val3));
  Serial.print(" val4 = " + String(val4));
  Serial.println(" val5 = " + String(val5));
*/

  // แปลงค่าจาก 0-1023 ให้เป็น 0-180
  val1 = map(val1, voltlow, volthigh, 0, 210);
  val2 = map(val2, voltlow, volthigh, 0, 210);
  val3 = map(val3, voltlow, volthigh, 0, 210);
  val4 = map(val4, voltlow, volthigh, 0, 210);
  val5 = map(val5, voltlow, volthigh, 0, 210);


  // แสดงผลค่าที่แปลงแล้ว
  Serial.print("val1 = " + String(val1));
  Serial.print(" val2 = " + String(val2));
  Serial.print(" val3 = " + String(val3));
  Serial.print(" val4 = " + String(val4));
  Serial.println(" val5 = " + String(val5));

  //servo_turn();

  delay(1000);  // หน่วงเวลา 1 วินาที
}


