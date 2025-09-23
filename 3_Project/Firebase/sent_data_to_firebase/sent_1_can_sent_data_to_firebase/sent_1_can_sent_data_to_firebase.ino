#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "Test_2"                  // ใส่ชื่อ Wi-Fi ของคุณ
#define WIFI_PASSWORD "987654321"           // ใส่รหัสผ่าน Wi-Fi ของคุณ

#define FIREBASE_HOST "project-name-e9690-default-rtdb.asia-southeast1.firebasedatabase.app"  // Firebase Host ของคุณ
#define FIREBASE_AUTH "Vu1tqyGQbUM1gFuNXbRFKWzp1ol37pritBCPJhuj"  // Firebase Database Secret ของคุณ

FirebaseData firebaseData;
bool state = false;
FirebaseConfig config; 

void setup() {
  Serial.begin(9600);
  connectWifi();

  // กำหนดค่าให้กับ FirebaseConfig object
  config.database_url = FIREBASE_HOST;  // ตั้งค่า Firebase Host
  config.signer.tokens.legacy_token = FIREBASE_AUTH;  // ใช้ Firebase Database Secret

  // เริ่มต้นการเชื่อมต่อกับ Firebase
  Firebase.begin(&config, NULL);  // ใช้ &config เพื่อส่งพอยน์เตอร์ไปยัง Firebase.begin()
}

void loop() {
  delay(1000);
  state = !state;
   
  if (Firebase.setBool(firebaseData, "/robot1/servo1", state)) {  // ส่งค่า state ไปที่ Firebase
    Serial.println("Added");
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
