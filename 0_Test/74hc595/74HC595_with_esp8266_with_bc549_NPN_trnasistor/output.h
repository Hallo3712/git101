#define led LED_BUILTIN
int led_high = 0;
int led_low  = 1;

int output_dl1 = 50;

void setup_output() {
  pinMode(led, OUTPUT);
}

void led_blink_end_setup() {
  Serial.println("led_blink_end_setup");
  for (int i; i <= 5; i++) {
    Serial.println("for is "+String(i));
    digitalWrite(led, led_high);
    delay(output_dl1);
    digitalWrite(led, led_low);
    delay(output_dl1);
  }
  Serial.println("out   led_blink_end_setup");
}
