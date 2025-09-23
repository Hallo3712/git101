#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "Test_2"                // ชื่อ Wi-Fi ของคุณ
#define WIFI_PASSWORD "987654321"         // รหัสผ่าน Wi-Fi ของคุณ

#define FIREBASE_HOST "project-name-e9690-default-rtdb.asia-southeast1.firebasedatabase.app"  // Firebase Host ของคุณ
#define FIREBASE_AUTH "Vu1tqyGQbUM1gFuNXbRFKWzp1ol37pritBCPJhuj"  // Firebase Database Secret ของคุณ

#define LED_PIN D8  // ใช้ Pin D8 สำหรับควบคุม LED

FirebaseData firebaseData;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);  // กำหนด Pin D8 เป็น OUTPUT สำหรับควบคุม LED
  connectWifi();

  // กำหนดค่าการเชื่อมต่อ Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // เริ่มต้นการเชื่อมต่อกับ Firebase
  Firebase.begin(&config, NULL); 
}

void loop() {
  // ดึงค่าของ "servo1" จาก Firebase path "/robot1/servo1"
  if (Firebase.getBool(firebaseData, "/robot1/servo1")) {
    bool state = firebaseData.boolData();  // อ่านค่าของ "servo1"

    // ตรวจสอบค่าของ state และควบคุม LED
    if (state) {
      digitalWrite(LED_PIN, HIGH);  // ถ้า state = true ให้ LED ติด
      Serial.println("LED ON");
    } else {
      digitalWrite(LED_PIN, LOW);   // ถ้า state = false ให้ LED ดับ
      Serial.println("LED OFF");
    }
  } else {
    // ถ้าเกิดข้อผิดพลาดในการดึงข้อมูล
    Serial.println("Error getting state: " + firebaseData.errorReason());
  }

  delay(1000);  // รอ 1 วินาทีเพื่อไม่ให้มีการร้องขอข้อมูลบ่อยเกินไป
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
