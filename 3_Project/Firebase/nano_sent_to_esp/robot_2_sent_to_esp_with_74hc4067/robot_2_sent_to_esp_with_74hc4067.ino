#include "var.h"
#include "function.h"
#include "74hc4067.h"


#include <SoftwareSerial.h>
SoftwareSerial espSerial(2, 3);  // RX TX

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

  Serial.println(".");
  Serial.println("end setup nano v1.1");
  Serial.println("");
  delay(100);

  setup_74hc4067();
}

void loop() {
  read_all_channel_74hc4067();
  //delay(0);
  Serial.print("val0 : " + String(val0));
  Serial.print(" val1 : " + String(val1));
  Serial.print(" val2 : " + String(val2));
  Serial.print(" val4 : " + String(val3));
  Serial.print(" val3 : " + String(val4));
  Serial.print(" val5 : " + String(val5));
  Serial.print(" val6 : " + String(val6));
  Serial.print(" val7 : " + String(val7));
  Serial.print(" val8 : " + String(val8));
  Serial.print(" val9 : " + String(val9));
  Serial.print(" val10 : " + String(val10));
  Serial.print(" val11 : " + String(val11));
  Serial.print(" val12 : " + String(val12));
  Serial.print(" val13 : " + String(val13));
  Serial.print(" val14 : " + String(val14));
  Serial.println(" val15 : " + String(val15));
  
  //delay(0);

  //espSerial.println(String(val0) + "," + String(val1) + "," + String(val2) + "," + String(val3) + "," + String(val4) + "," + String(val5));
  //Serial.println("done");
  delay(1000);//ค่าตรง
  //delay(500); //ตรงอยู่
}
