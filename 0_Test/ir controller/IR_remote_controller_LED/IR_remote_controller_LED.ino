#include <DIYables_IRcontroller.h>  // DIYables_IRcontroller library
#define IR_RECEIVER_PIN 11  // The Arduino pin connected to IR controller
DIYables_IRcontroller_21 irController(IR_RECEIVER_PIN, 200);  // debounce time is 200ms

#define led 3

char translate_IR_data;
#include "Translate_IR.h"

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  irController.begin();
}

void loop() {
  translate_IR();
}

