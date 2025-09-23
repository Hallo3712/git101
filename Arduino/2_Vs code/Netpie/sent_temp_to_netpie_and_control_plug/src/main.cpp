#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ArduinoJson.h> // เพิ่มไลบรารีสำหรับจัดการ JSON

// ====== WiFi & MQTT Config ======
const char* ssid = "Test";
const char* password = "Test987654321";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;

// *** ใช้ข้อมูล Device ของคุณ: esp8266_1 (จาก Project do_it_myself) ***
const char* mqtt_Client = "04a9bf82-326a-4b95-a6e7-260d3e87e7cc"; 
const char* mqtt_username = "Wq7BxGDMXpa7up7Wg2WgnyCXHVBVeJY3"; 
const char* mqtt_password = "EgX9tJwgv8aLVVvqB31CHuN5Aw2bMXeD"; 

WiFiClient espClient;
PubSubClient client(espClient);

#define led LED_BUILTIN         // GPIO2 (Active LOW)
#define DHTPIN D5               // GPIO14 (D5) สำหรับ DHT11
// #define RELAY_PIN D6            // GPIO12 (D6) สำหรับควบคุม Relay - ตัดออกไปตามที่ต้องการ

DHT dht;

int c1 = 0; // ตัวนับรอบ

// Topic สำหรับรับการเปลี่ยนแปลงของ Shadow จาก NETPIE
const char* SHADOW_UPDATE_TOPIC = "@shadow/data"; 

// Topic สำหรับร้องขอค่า Shadow ปัจจุบัน (ใช้เมื่อเชื่อมต่อ MQTT ได้ครั้งแรก)
const char* SHADOW_GET_TOPIC = "@shadow/get";

// Topic สำหรับส่งค่า DHT Sensor ไปยัง Shadow
const char* SHADOW_PUBLISH_TOPIC = "@shadow/data/update";

// ====== Function to process incoming Shadow data ======
void processShadowData(String payload) {
  Serial.println("Processing Shadow Payload:");
  Serial.println(payload);

  StaticJsonDocument<512> doc; // ขนาด Document อาจปรับเพิ่มตามความซับซ้อนของ Shadow
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject desired_state;
  if (doc.containsKey("state") && doc["state"].containsKey("desired")) {
    desired_state = doc["state"]["desired"];
    Serial.println("Found 'state.desired' in Shadow payload.");
  } else if (doc.containsKey("desired")) {
    desired_state = doc["desired"];
    Serial.println("Found 'desired' in Shadow payload.");
  } else {
    // ถ้าไม่พบ desired state ในโครงสร้างที่คาดไว้ ลองดู reported state หรืออื่นๆ
    Serial.println("No 'desired' state found in Shadow payload. Checking other fields.");
    if (doc.containsKey("state") && doc["state"].containsKey("reported")) {
      JsonObject reported_state = doc["state"]["reported"];
      if (reported_state.containsKey("data")) {
        Serial.print("Reported data (from DHT): ");
        serializeJson(reported_state["data"], Serial);
        Serial.println();
      }
    }
    // สำหรับการตอบกลับจาก @shadow/get, ค่ามักจะอยู่ใน "data" หรือ "state" โดยตรง
    if (doc.containsKey("data")) {
        Serial.print("Data from Shadow (likely from @shadow/get response): ");
        serializeJson(doc["data"], Serial);
        Serial.println();
    }
    return; // ออกจากฟังก์ชันถ้าไม่ใช่โครงสร้างที่คาดหวังสำหรับคำสั่ง
  }

  // === อ่านค่า Mode ต่างๆ จาก Desired State และแสดงผล ===
  if (desired_state.containsKey("mode")) {
    int mode_val = desired_state["mode"].as<int>();
    Serial.print("Shadow Desired 'mode' received: ");
    Serial.println(mode_val);
  } else {
    Serial.println("Field 'mode' not found in desired state.");
  }

  if (desired_state.containsKey("mode_A")) {
    int mode_A_val = desired_state["mode_A"].as<int>();
    Serial.print("Shadow Desired 'mode_A' received: ");
    Serial.println(mode_A_val);
  } else {
    Serial.println("Field 'mode_A' not found in desired state.");
  }

  if (desired_state.containsKey("mode_B")) {
    int mode_B_val = desired_state["mode_B"].as<int>();
    Serial.print("Shadow Desired 'mode_B' received: ");
    Serial.println(mode_B_val);
  } else {
    Serial.println("Field 'mode_B' not found in desired state.");
  }
}

// ====== Callback for incoming MQTT messages ======
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) { // ใช้ unsigned int สำหรับ i
    payloadStr += (char)payload[i];
  }
  Serial.println(payloadStr);

  // ถ้าข้อความมาจาก Shadow Topic ให้ Process Shadow data
  if (String(topic) == SHADOW_UPDATE_TOPIC) {
    processShadowData(payloadStr);
  } 
  // ส่วนนี้ถูกคอมเมนต์ไว้ เพราะคุณต้องการตัดการควบคุม Relay ผ่าน @msg/test2 ออกไปก่อน
  /*
  else if (String(topic) == "@msg/test2") {
    if (payloadStr == "on") {
      // digitalWrite(RELAY_PIN, HIGH); 
      Serial.println("🔔 Received 'on' from @msg/test2 (Relay control commented out)");
    } else if (payloadStr == "off") {
      // digitalWrite(RELAY_PIN, LOW);   
      Serial.println("🔕 Received 'off' from @msg/test2 (Relay control commented out)");
    }
  }
  */
}

// ====== MQTT reconnect function ======
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // เมื่อเชื่อมต่อแล้ว ให้ Subscribe เพื่อรับการอัปเดต Shadow
      if (client.subscribe(SHADOW_UPDATE_TOPIC)) {
        Serial.print("Subscribed to Shadow Topic: ");
        Serial.println(SHADOW_UPDATE_TOPIC);
      } else {
        Serial.print("Failed to subscribe to Shadow Topic, rc=");
        Serial.println(client.state());
      }
      // ร้องขอค่า Shadow ปัจจุบันทันทีเมื่อเชื่อมต่อได้
      // เพื่อให้อุปกรณ์ได้สถานะล่าสุดหากมีการเปลี่ยนแปลงขณะออฟไลน์
      client.publish(SHADOW_GET_TOPIC, ""); 
      Serial.println("Requested current Shadow data.");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- ESP8266 Shadow Read Test (Mode Values) ---");
  Serial.println("Connecting to WiFi...");

  pinMode(led, OUTPUT);
  // pinMode(RELAY_PIN, OUTPUT); // ตัดออกไปตามที่ต้องการ
  
  // digitalWrite(RELAY_PIN, LOW); // เริ่มต้นปิดรีเลย์ - ตัดออกไปตามที่ต้องการ
  digitalWrite(led, HIGH); // ปิด LED_BUILTIN (Active LOW)

  // กระพริบ LED ถี่ 5 ครั้ง
  for (int i = 0; i < 5; i++) {
    digitalWrite(led, LOW);    // เปิดไฟ (LED_BUILTIN เป็น Active LOW)
    delay(100);
    digitalWrite(led, HIGH);   // ปิดไฟ (LED_BUILTIN เป็น Active LOW)
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
    Serial.println("MQTT Disconnected. Reconnecting...");
    reconnect();
  }
  client.loop(); // ต้องทำงานตลอดเวลาเพื่อประมวลผลข้อความ MQTT ที่เข้ามา

  static unsigned long lastSendTime = 0;
  const unsigned long sendInterval = 5000; // ส่งทุก 5 วินาที

  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis();
    c1++;

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    String status = "Read";

    if (isnan(humidity) || isnan(temperature)) {
      status = "Nan";
      Serial.println("❌ Failed to read from DHT sensor, skipping data transmission");

      String data = "{\"data\" : {\"count\" :" + String(c1) + ", \"status\" : \"" + status + "\"}}";
      Serial.println("Publishing to Shadow: " + data);
      client.publish(SHADOW_PUBLISH_TOPIC, data.c_str());
      return;
    }

    Serial.println("✅ Read successful");
    Serial.println("=== Sensor Readings ===");
    Serial.print("Count: "); Serial.println(c1);
    Serial.print("Temperature: "); Serial.print(temperature, 1); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(humidity, 1); Serial.println(" %");
    Serial.print("Status: "); Serial.println(status);

    // สร้าง JSON String สำหรับส่งไป Shadow
    StaticJsonDocument<256> doc_out; 
    JsonObject data_obj = doc_out.createNestedObject("data");
    data_obj["count"] = c1;
    data_obj["temp"] = temperature;
    data_obj["humidity"] = humidity;
    data_obj["status"] = status;

    String json_output;
    serializeJson(doc_out, json_output);
    Serial.println("Publishing to Shadow: " + json_output);
    client.publish(SHADOW_PUBLISH_TOPIC, json_output.c_str());
  }
}