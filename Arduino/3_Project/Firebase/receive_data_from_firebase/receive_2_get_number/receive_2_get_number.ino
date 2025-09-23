#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include "pca9685.h"

#define WIFI_SSID "Test_2"                // ชื่อ Wi-Fi ของคุณ
#define WIFI_PASSWORD "987654321"         // รหัสผ่าน Wi-Fi ของคุณ

#define FIREBASE_HOST "project-name-e9690-default-rtdb.asia-southeast1.firebasedatabase.app"  // Firebase Host ของคุณ
#define FIREBASE_AUTH "Vu1tqyGQbUM1gFuNXbRFKWzp1ol37pritBCPJhuj"  // Firebase Database Secret ของคุณ

#define LED_PIN D8  // ใช้ Pin D8 สำหรับควบคุม LED

FirebaseData firebaseData;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);  // กำหนด Pin D8 เป็น OUTPUT สำหรับควบคุม LED
  connectWifi();

  // กำหนดค่าการเชื่อมต่อ Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // เริ่มต้นการเชื่อมต่อกับ Firebase
  Firebase.begin(&config, NULL); 

  setup_pc9685();

}

void loop() {
  // ดึงค่าของ "servo1" จาก Firebase path "/robot1/servo1"
  if (Firebase.getInt(firebaseData, "/robot1/servo0")) {
    p0 = firebaseData.intData();  // อ่านค่าของ "servo1"
    Serial.println("p0 is "+String(p0));
    servo_turn();
    
  } else {
    // ถ้าเกิดข้อผิดพลาดในการดึงข้อมูล
    Serial.println("Error getting state: " + firebaseData.errorReason());
  }

  delay(10);  // รอ 1 วินาทีเพื่อไม่ให้มีการร้องขอข้อมูลบ่อยเกินไป
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
