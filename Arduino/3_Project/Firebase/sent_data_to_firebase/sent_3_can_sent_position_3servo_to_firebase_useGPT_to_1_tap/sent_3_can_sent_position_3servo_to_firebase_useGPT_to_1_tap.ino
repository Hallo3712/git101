#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include <SoftwareSerial.h>
SoftwareSerial nanoSerial(D5, D6);  // RX TX

#include "var.h"

// ประกาศตัวแปรทั้งหมดจากแท็บที่สอง
String data = "";
int result = 0;
int val0 = 0;
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;

// กำหนดค่า Wi-Fi และ Firebase
#define WIFI_SSID "Test_2"         // ใส่ชื่อ Wi-Fi ของคุณ
#define WIFI_PASSWORD "987654321"  // ใส่รหัสผ่าน Wi-Fi ของคุณ

#define FIREBASE_HOST "project-name-e9690-default-rtdb.asia-southeast1.firebasedatabase.app"  // Firebase Host ของคุณ
#define FIREBASE_AUTH "Vu1tqyGQbUM1gFuNXbRFKWzp1ol37pritBCPJhuj"                              // Firebase Database Secret ของคุณ

FirebaseData firebaseData;
int num = 0;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  nanoSerial.begin(115200);

  connectWifi();

  // กำหนดค่าให้กับ FirebaseConfig object
  config.database_url = FIREBASE_HOST;                // ตั้งค่า Firebase Host
  config.signer.tokens.legacy_token = FIREBASE_AUTH;  // ใช้ Firebase Database Secret

  // เริ่มต้นการเชื่อมต่อกับ Firebase
  Firebase.begin(&config, NULL);  // ใช้ &config เพื่อส่งพอยน์เตอร์ไปยัง Firebase.begin()

  Serial.println(".");
  Serial.println("end setup esp v1.1");
  Serial.println("");
  delay(100);
}

void loop() {
  delay(10);
  /*
  num = num += 1;
  if (num >= 100)
    num = 0;
  Serial.println("num is " + String(num));

  if (Firebase.setInt(firebaseData, "/robot1/servo1", num)) {  // ส่งค่า state ไปที่ Firebase
    //Serial.println("Added");
  } else {
    Serial.println("Error: " + firebaseData.errorReason());
  }
  */

  if (nanoSerial.available()) {
    data = nanoSerial.readStringUntil('\n');
    Serial.println("data is " + String(data));

    result = sscanf(data.c_str(), "%d,%d,%d,%d,%d", &val0, &val1, &val2, &val3, &val4);
    Serial.println("result is " + String(result));

    Serial.println("val0 is " + String(val0) + " val1 is " + String(val1) + " val2 is " + String(val2) /*+ " val3 is " + String(val3) + " val4 is " + String(val4)*/);
  }

  if (Firebase.setInt(firebaseData, "/robot1/servo0", val0)) {  // ส่งค่า state ไปที่ Firebase
    //Serial.println("Added");
  } else {
    Serial.println("Error: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/robot1/servo1", val1)) {  // ส่งค่า state ไปที่ Firebase
    //Serial.println("Added");
  } else {
    Serial.println("Error: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/robot1/servo2", val2)) {  // ส่งค่า state ไปที่ Firebase
    //Serial.println("Added");
  } else {
    Serial.println("Error: " + firebaseData.errorReason());
  }
}

void connectWifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}
