#include "RTCDS1307.h"

RTCDS1307 rtc(0x68);

uint8_t year, month, weekday, day, hour, minute, second;
bool period = 0;

String m[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Out", "Nob", "Dec"};
String w[7] = { "Saturday ","Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

void setup()
{
  Serial.begin(115200);
  rtc.begin();
  rtc.setDate(24, 7, 30);
  rtc.setTime(16, 23, 00);
}

void loop()
{
  rtc.getDate(year, month, day, weekday);
  rtc.getTime(hour, minute, second, period);
  //if (!(second % 3)) rtc.setMode(1 - rtc.getMode());
  //rtc.getTime(hour, minute, second, period);

  Serial.print(w[weekday]);
  Serial.print("  ");
  Serial.print(day, DEC);
  Serial.print("/");
  Serial.print(m[month]);
  Serial.print("/");
  Serial.print(year + 2000, DEC);
  Serial.print("  ");
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  
  Serial.print(rtc.getMode() ? (period ? " PM" : " AM") : "");
  
  Serial.println();
  delay(1000);
  
}
