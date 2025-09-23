#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ====== WiFi & MQTT Config ======
const char* ssid = "Test";
const char* password = "Test987654321";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "04a9bf82-326a-4b95-a6e7-260d3e87e7cc";
const char* mqtt_username = "Wq7BxGDMXpa7up7Wg2WgnyCXHVBVeJY3";
const char* mqtt_password = "EgX9tJwgv8aLVVvqB31CHuN5Aw2bMXeD";

WiFiClient espClient;
PubSubClient client(espClient);

#define led LED_BUILTIN     // บน ESP8266 12E ปกติจะเป็น GPIO2 (Active LOW)
#define DHTPIN D5           // ต่อ DHT ที่ GPIO14 (D5)
DHT dht;

int c1 = 0; // ตัวนับรอบ

// ====== MQTT reconnect ======
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/test2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// ====== Callback for incoming MQTT messages ======
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  Serial.println("\nConnecting to WiFi...");

  pinMode(led, OUTPUT);

  // กระพริบ LED ถี่ 5 ครั้ง
  for (int i = 0; i < 5; i++) {
    digitalWrite(led, LOW);   // เปิดไฟ
    delay(100);
    digitalWrite(led, HIGH);  // ปิดไฟ
    delay(100);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected. IP address:");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  dht.setup(DHTPIN); // เริ่มใช้งาน DHT
}

// ====== Main Loop ======

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static unsigned long lastSendTime = 0;
  const unsigned long sendInterval = 5000; // ส่งทุก 5 วินาที

  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis();
    c1++;

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    String status = "Read"; // กำหนดสถานะเป็น "Read" เริ่มต้น

    // ถ้าอ่านค่าไม่ได้ (ค่าเป็น nan), กำหนดสถานะเป็น "Nan" และไม่ส่งค่า temp, hum
    if (isnan(humidity) || isnan(temperature)) {
      status = "Nan";
      Serial.println("❌ Failed to read from DHT sensor, skipping data transmission");
      
      // ไม่ส่งข้อมูล temp และ hum
      String data = "{\"data\" : {\"count\" :" + String(c1) + ", \"status\" : \"" + status + "\"}}";
      Serial.println(data);
      client.publish("@shadow/data/update", data.c_str());
      return;  // ข้ามการส่งข้อมูล
    }

    // แสดงผล
    Serial.println("=== Sensor Readings ===");
    Serial.print("Count: "); Serial.println(c1);
    Serial.print("Temperature: "); Serial.print(temperature, 1); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(humidity, 1); Serial.println(" %");
    Serial.print("Status: "); Serial.println(status);

    // สร้าง JSON และส่งไป
    String data = "{\"data\" : {\"count\" :" + String(c1) + ", \"temp\" :" + String(temperature, 1) + ", \"humuduty\" :" + String(humidity, 1) + ", \"status\" : \"" + status + "\"}}";

    Serial.println(data);
    client.publish("@shadow/data/update", data.c_str());
  }
}

