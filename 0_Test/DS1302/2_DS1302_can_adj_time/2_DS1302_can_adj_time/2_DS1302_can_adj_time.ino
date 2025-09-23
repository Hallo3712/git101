int Buzzer = LED_BUILTIN;
#include <DS1302.h>

// Init the DS1302
DS1302 rtc(2, 3, 4);

// Init a Time-data structure
Time t;

void setup() {
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // Setup Serial connection
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, HIGH);
  Serial.begin(9600);

  // The following lines can be commented out to use the values already stored in the DS1302
  //rtc.setDOW(TUESDAY);
  //rtc.setTime(21, 34, 00);
  //rtc.setDate(3, 4, 2024);
}

void loop() {
  // Get data from the DS1302
  t = rtc.getTime();

  // Send date over serial connection
  Serial.print("Today is  ");
  Serial.print(t.dow);
  Serial.print(t.date, DEC);
  //Serial.print(" Day,");
  Serial.print("/");
  Serial.print(rtc.getMonthStr());
  //Serial.print(" Month,");
  Serial.print("/");
  Serial.print(t.year, DEC);
  //Serial.println(" year");
  Serial.print("\t");

  // Send Day-of-Week and time
  Serial.print(t.hour, DEC);
  //Serial.print(" hour,");
  Serial.print(" : ");
  Serial.print(t.min, DEC);
  //Serial.print(" minute,");
  Serial.print(" : ");
  Serial.println(t.sec, DEC);
  //Serial.println(" second");

  // Send a divider for readability
  //Serial.println(" - - - - - - - - - - - - - - - - - - - - -");

  // Wait one second before repeating :)
  delay(1000);
  if (t.hour == 15 && t.min == 45 && t.sec == 0) {
    Serial.println("Alarm Start");
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    Serial.println("Alarm End");
  }
}