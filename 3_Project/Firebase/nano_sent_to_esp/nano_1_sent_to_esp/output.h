#define led LED_BUILTIN

int led_st_high = 1;
int led_st_low = 0;

int output_dl1 = 50;

void setup_output() {
  pinMode(led, OUTPUT);
}

void led_blink_end_setup() {
  Serial.println("get in   led_blink_end_setup");
  for (int i=0; i <= 5; i++) {
    Serial.println("for is " + String(i));
    digitalWrite(led, led_st_high);
    delay(output_dl1);
    digitalWrite(led, led_st_low);
    delay(output_dl1);
  }
  Serial.println("get out   led_blink_end_setup");
}

void led_high() {
  digitalWrite(led, led_st_high);
  Serial.println("led is high");
  delay(output_dl1);
}
void led_low() {
  digitalWrite(led, led_st_low);
  Serial.println("led is low");
  delay(output_dl1);
}
