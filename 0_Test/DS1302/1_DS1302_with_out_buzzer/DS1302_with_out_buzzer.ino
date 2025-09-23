#include <DS1302.h>

// Init the DS1302
DS1302 rtc(2, 3, 4);

// Init a Time-data structure
Time t;

void setup()
{
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // Setup Serial connection
  pinMode (Buzzer, OUTPUT);
  digitalWrite(Buzzer, HIGH);
  Serial.begin(9600);

  // The following lines can be commented out to use the values already stored in the DS1302
   rtc.setDOW(FRIDAY);
   rtc.setTime(15, 40, 50);
   rtc.setDate(21, 10, 2022);
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
  
}