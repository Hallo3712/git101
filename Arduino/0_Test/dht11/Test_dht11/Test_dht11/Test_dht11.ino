#include "DHT.h"
DHT dht;
void setup()
{
Serial.begin(9600);
Serial.println();
Serial.println("Status\tความชื้น (%)\tอุณหภูมิ (C)\t(F)");
 ฟ
dht.setup(D5); //
}
 
void loop()
{
delay(dht.getMinimumSamplingPeriod());
float humidity = dht.getHumidity(); // ดึงค่าความชื้น
float temperature = dht.getTemperature(); // ดึงค่าอุณหภูมิ
Serial.print(dht.getStatusString());
Serial.print("\t");
Serial.print(humidity, 1);
Serial.print("\t\t");
Serial.print(temperature, 1);
Serial.print("\t\t");
Serial.println(dht.toFahrenheit(temperature), 1);
delay(1000);
}