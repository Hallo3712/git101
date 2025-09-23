#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ==== Wi-Fi & Firebase ====
#define WIFI_SSID "Test"
#define WIFI_PASSWORD "Test987654321"
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

// ==== PCA9685 ====
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void setup() {
  Serial.begin(115200);

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // ตั้งค่า Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);

  // ตั้งค่า PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    String basePath = "/robot1/servo" + String(i);

    // อ่านค่า angle
    int angle = 90; // ค่าเริ่มต้น fallback
    if (Firebase.getInt(firebaseData, basePath + "/angle")) {
      angle = firebaseData.intData();
    } else {
      Serial.println("Error getting angle for servo" + String(i) + ": " + firebaseData.errorReason());
    }

    // อ่านค่า servoMin
    int sMin = 320;
    if (Firebase.getInt(firebaseData, basePath + "/servoMin")) {
      sMin = firebaseData.intData();
    } else {
      Serial.println("Error getting servoMin for servo" + String(i) + ": " + firebaseData.errorReason());
    }

    // อ่านค่า servoMax
    int sMax = 860;
    if (Firebase.getInt(firebaseData, basePath + "/servoMax")) {
      sMax = firebaseData.intData();
    } else {
      Serial.println("Error getting servoMax for servo" + String(i) + ": " + firebaseData.errorReason());
    }

    // แปลง angle ไปเป็นค่าพัลส์และส่งให้ PCA9685
    int pulse = map(angle, 0, 180, sMin, sMax);
    pwm.setPWM(i, 0, pulse);
  }

  delay(500);  // ป้องกันการโหลดข้อมูลถี่เกินไป
}
