int time1;
int time2;
#define led1 2
int dl_led1 = 1000;

int val;


void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
}

void loop() {
  Serial.println("start loop");
  digitalWrite(led1, HIGH);
  Serial.println("led high");
  time1 = millis();

  delay(700);
  Serial.println("Work two");

  time2 = millis();

  val = time1 + time2;

  if (val = dl_led1) {
    Serial.println("1000");
    digitalWrite(led1, LOW);
    Serial.println("led low");

  } else if (val < dl_led1) {
    Serial.println("else if");
    int val1 = dl_led1 - val;
    delay(val1);
    digitalWrite(led1, LOW);
    Serial.println("led low");
  }
  delay(dl_led1);
}
