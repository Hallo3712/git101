#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// LED
int open_min = 0;
int open_max = 3900;
int step_fade = 200;
int delay_fade = 100;

void setup() {
  Serial.begin(9600);
  pwm.begin();

  // set pwm frequency = 1000 Hz
  pwm.setPWMFreq(50);
  Serial.println("start pwm");
  delay(1000);
}

void loop() {
  /*
  pwm_led(0);
  pwm_led(25);
  pwm_led(50);
  pwm_led(75);
  pwm_led(100);
*/
  fade_led();
}

void pwm_led(int p)  // pwm_led(%);
{
  Serial.println("p is " + String(p) + " %");
  int p1 = map(p, 0, 100, open_min, open_max);
  Serial.println("p1 is " + String(p1) + " Hz");

  pwm.setPWM(0, 0, p1);
  delay(1000);
}

void fade_led()     // fede_led();
{
  int i;
  for(i ; i <= open_max; i += step_fade){
    Serial.println("i is " + String(i));
    pwm.setPWM(0,0,i);
    delay(delay_fade);
  }
  for(i ;i >= open_min; i -= step_fade){
    Serial.println("i is " + String(i));
    pwm.setPWM(0,0,i);
    delay(delay_fade);
  }
}








