int Buzzer = 5;
#include <DS1302.h>

// Init the DS1302
DS1302 rtc(6,7,8);

// Init a Time-data structure
Time t;

void setup()
{
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // Setup Serial connection
  pinMode (Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  Serial.begin(9600);

  // The following lines can be commented out to use the values already stored in the DS1302
  //rtc.setDOW(THURSDAY);
  //rtc.setTime(16, 16, 00);
  //rtc.setDate(21, 2, 2024);
}

void loop()
{
  // Get data from the DS1302
  t = rtc.getTime();

  // Send date over serial connection
  Serial.print("Today is the ");
  Serial.print(t.date);
  Serial.print(" Day,");
  Serial.print(rtc.getMonthStr());
  Serial.print(" Month,");
  Serial.print(t.year);
  Serial.println(" year");

  // Send Day-of-Week and time
  Serial.print(t.hour);
  Serial.print(" hour,");
  Serial.print(t.min);
  Serial.print(" minute,");
  Serial.print(t.sec);
  Serial.println(" second");

  // Send a divider for readability
  Serial.println(" - - - - - - - - - - - - - - - - - - - - -");




  // Wait one second before repeating :)
  delay (1000);
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