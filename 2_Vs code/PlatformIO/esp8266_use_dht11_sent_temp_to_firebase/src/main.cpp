#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h> // <--- ตัวนี้แหละครับ
#include <DHT.h>

// --- WiFi & Firebase Configuration ---
#define WIFI_SSID "Test"        // ชื่อ WiFi ของคุณ
#define WIFI_PASSWORD "Test987654321"  // รหัสผ่าน WiFi ของคุณ

// สำหรับ Firebase-ESP8266 ตัวเก่า ใช้ URL เต็ม
#define FIREBASE_HOST "https://app-firebase-servo-default-rtdb.asia-southeast1.firebasedatabase.app/" // URL ของ Firebase RTDB
#define FIREBASE_AUTH "ak7LKZApYAGTP5HBzClMHVHDRwyRmXP5wY6QTBPX" // Web API Key ของคุณ

// --- DHT11 Sensor Configuration ---
#define DHTPIN D6         // Pin ที่เชื่อมต่อ DHT11 (GPIO4 หรือ D2 บน NodeMCU)
#define DHTTYPE DHT11     // ประเภทของเซ็นเซอร์ DHT (DHT11 หรือ DHT22)

DHT dht(DHTPIN, DHTTYPE);
FirebaseData firebaseData; // <--- ตัวนี้แหละครับ

void setup() {
    Serial.begin(115200);
    Serial.println();

    // Connect to WiFi
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize DHT sensor
    dht.begin();
    Serial.println("DHT11 sensor initialized.");

    // Initialize Firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // <--- ตัวนี้แหละครับ
    Firebase.reconnectWiFi(true); // Reconnect WiFi automatically
    Serial.println("Firebase initialized.");
}

void loop() {
    delay(5000); // Read sensor every 5 seconds

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print("    Temperature: ");
    Serial.print(t);
    Serial.println(" *C");

    // Send data to Firebase
    String path = "/sensor_data"; // Path ใน Firebase RTDB ที่จะเก็บข้อมูล

    // Create JSON object for data
    FirebaseJson json;
    json.set("temperature", String(t, 2)); // เก็บค่าทศนิยม 2 ตำแหน่ง
    json.set("humidity", String(h, 2));
    json.set("timestamp", Firebase.getCurrentTimestamp()); // เพิ่ม Timestamp

    Serial.print("Sending data to Firebase... ");
    if (Firebase.setJson(firebaseData, path, json)) {
        Serial.println("PASSED");
    } else {
        Serial.print("FAILED: ");
        Serial.println(firebaseData.errorReason());
    }
}