#include <SoftwareSerial.h>
SoftwareSerial espSerial(2, 3);  // RX TX

#include "var.h"
#include "input.h"
#include "output.h"
//#include "74hc595.h"
#include "function_1.h"




void setup() {
  setup_input();
  Serial.begin(115200);
  espSerial.begin(115200);
  setup_output();
  led_blink_end_setup();

  Serial.println(".");
  Serial.println("end setup v1.3");
  Serial.println("");
  delay(100);
}

void loop() {
  //Serial.println("get start");
  if_serial_available();
  //read_print_a0();
  //map_value();
  
  for(int i=0; i<=2; i++){
    Serial.println("i is "+String(i));
    read_and_get_value_from_channal(i);
  }
  sent_to_esp();



  delay(300);
}
