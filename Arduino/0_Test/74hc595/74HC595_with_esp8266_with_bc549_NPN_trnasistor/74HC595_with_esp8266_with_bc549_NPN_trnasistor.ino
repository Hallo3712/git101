#include "var.h"
#include "input.h"
#include "output.h"
#include "74hc595.h"


void setup() { 
  setup_sw();
  Serial.begin(115200);
  setup_output();
  led_blink_end_setup();

  Serial.println(""); 
  Serial.println("end setup");
  Serial.println(""); 
  delay(100);
  
}

void loop() {
  for(int i = 1; i <= 2; i++ ){
    Serial.println("i is "+String(i));
    count = i;
    Serial.println("count is "+String(count));
    trick();
    read_print_a0();
    delay(1000);
  }
}










