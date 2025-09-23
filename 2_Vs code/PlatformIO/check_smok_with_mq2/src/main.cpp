#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int mq2AnalogPin = A0;
const int mq2DigitalPin = 2;  // ใช้ขา D2 สำหรับอ่าน Digital Output จาก D0 ของ MQ2

LiquidCrystal_I2C lcd(0x27, 16, 2);  // ตั้งที่อยู่ I2C 0x27 จอ 16x2

void setup() {
  pinMode(mq2DigitalPin, INPUT);
  lcd.init();
  lcd.backlight();
  delay(2000);
}

void loop() {
  int analogValue = analogRead(mq2AnalogPin);
  int digitalValue = digitalRead(mq2DigitalPin);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A0: ");
  lcd.print(analogValue);

  lcd.setCursor(0, 1);
  lcd.print("D0: ");
  lcd.print(digitalValue);

  delay(1000);
}
