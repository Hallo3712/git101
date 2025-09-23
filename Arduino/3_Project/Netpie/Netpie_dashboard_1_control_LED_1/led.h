#define led D2
bool bool_led_st;
int d1 = 1000;



void blink_led(int time) // blink_led(int_time);
{
  for( int i; i<=time; i++){
    digitalWrite(led,HIGH);
    delay(d1);
    digitalWrite(led,LOW);
    delay(d1);
  }
}