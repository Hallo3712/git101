#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ข้อมูล Wi-Fi และ Firebase
#define WIFI_SSID "Test_2"
#define WIFI_PASSWORD "987654321"

#define FIREBASE_HOST "project-name-e9690-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "Vu1tqyGQbUM1gFuNXbRFKWzp1ol37pritBCPJhuj"

#define LED_PIN D8  // Pin สำหรับควบคุม LED (ถ้าต้องการใช้งาน)

FirebaseData firebaseData;
FirebaseConfig config;

// การกำหนด PWM สำหรับเซอร์โว
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define servosetzero 200
#define servoMin 320  // Pulse น้อยสุด
#define servoMax 860  // Pulse มากสุด
#define servoStep 10

int p0 = 0;  // Position ของ servo 0
int p1 = 0;  // Position ของ servo 1
int p2 = 0;  // Position ของ servo 2
int dl_1 = 10;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);  // กำหนด Pin D8 เป็น OUTPUT สำหรับควบคุม LED

  connectWifi();

  // กำหนดค่าการเชื่อมต่อ Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // เริ่มต้นการเชื่อมต่อกับ Firebase
  Firebase.begin(&config, NULL);

  pwm.begin();
  pwm.setPWMFreq(60);  // กำหนดความถี่ของ PWM
}

void loop() {
  delay(10);  // รอเพื่อไม่ให้มีการร้องขอข้อมูลบ่อยเกินไป

  // ดึงค่าของ "servo0" จาก Firebase
  if (Firebase.getInt(firebaseData, "/robot1/servo0")) {
    p0 = firebaseData.intData();  // อ่านค่าของ "servo0"
    Serial.println("p0 is " + String(p0));
  } else {
    Serial.println("Error getting state: " + firebaseData.errorReason());
  }

  // ดึงค่าของ "servo1"
  if (Firebase.getInt(firebaseData, "/robot1/servo1")) {
    p1 = firebaseData.intData();  // อ่านค่าของ "servo1"
    Serial.println("p1 is " + String(p1));
  } else {
    Serial.println("Error getting state: " + firebaseData.errorReason());
  }

  // ดึงค่าของ "servo2"
  if (Firebase.getInt(firebaseData, "/robot1/servo2")) {
    p2 = firebaseData.intData();  // อ่านค่าของ "servo2"
    Serial.println("p2 is " + String(p2));
  } else {
    Serial.println("Error getting state: " + firebaseData.errorReason());
  }

  // ควบคุมการหมุนเซอร์โวตามค่าที่ได้จาก Firebase
  servo_turn(0, p0);
  servo_turn(1, p1);
  servo_turn(2, p2);
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

void servo_turn(int servo, int position) {
  switch (servo) {
    case 0:
      position = map(position, 0, 180, servoMin, servoMax);
      pwm.setPWM(servo, servosetzero, position);
      Serial.println("Servo 0 position: " + String(position));
      break;
    case 1:
      position = map(position, 0, 180, servoMin, servoMax);
      pwm.setPWM(servo, servosetzero, position);
      Serial.println("Servo 1 position: " + String(position));
      break;
    case 2:
      position = map(position, 0, 180, servoMin, servoMax);
      pwm.setPWM(servo, servosetzero, position);
      Serial.println("Servo 2 position: " + String(position));
      break;
  }
  delay(dl_1);
}
