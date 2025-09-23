#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "val.h"
#include "publish.h"
#include "led.h"


const char* ssid = "test";
const char* password = "987654321";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "a3ca77c6-1a22-4e9e-bb9e-89a0dce52060";
const char* mqtt_username = "s8XHdF2YuTY9FYikwhRZebTymfqa8EV6";
const char* mqtt_password = "LRhAx6z9bR94UycpssSxWnYR3WUPFNek";

WiFiClient espClient;
PubSubClient client(espClient);


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/button");
      client.subscribe("@msg/slide");
      client.subscribe("@msg/slide_value");


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println("message is " + message);
  if (String(topic) == "@msg/button") {  // รับค่าการกดปุ่ม
    value_msg_button = message;
    Serial.println("@msg/button is " + String(value_msg_button));
  } else if (String(topic) == "@msg/slide") {  // รับค่าการเปิด/ปิด slide
    Serial.println("@msg/slide is " + String(message));
    if (message == "on_slide") {
      slide = 1;
    } else if (message == "off_slide") {
      slide = 0;

    } else if (slide == 1) {  // check slide
      Serial.println("slide is " + String(slide));
      slide_value = map(slide_value, 0, 100, 0, 5);
      Serial.println("slide_value = map is " + String(slide_value));
      analogWrite(led, slide_value);
      delay(100);
      Serial.println("analogwrite is " + String(slide_value));
    }
    else {
      Serial.println("message slide is not correct (on_slide/off_slide)");
    }
    Serial.println("slide is " + String(slide));
    Serial.println("slide_value is " + String(slide_value));

  } else if (String(topic) == "@msg/slide_value") {  // รับค่าการปรับตั้ง slide
    Serial.println("@msg/slide_value is " + String(slide_value));
    slide_value = message.toInt();
    Serial.println("slide_value = message.toInt is " + String(slide_value));

    // check slide
    if (slide == 1) {
      Serial.println("slide is " + String(slide));
      slide_value = map(slide_value, 0, 100, 0, 5);
      Serial.println("slide_value = map is " + String(slide_value));
      analogWrite(led, slide_value);
      Serial.println("analogwrite is " + String(slide_value));
    }
  }
}



void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(led, OUTPUT);
  blink_led(5);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // check button
  if (value_msg_button != "0")  // เช็คค่าที่ส่งเข้ามาเพื่อเอาไปทำงาน
  {
    incoming += 1;
    if (value_msg_button == "on") {
      digitalWrite(led, 1);
      delay(100);
    } else if (value_msg_button == "off") {
      digitalWrite(led, 0);
      delay(100);
    } else if (value_msg_button == "sent all data") {
      sent_all_data_to_netpie(1000);
    }
    value_msg_button = "0";  // เมื่อใช้ค่าที่ส่งมาเสร็จแล้วให้ทำการคืนค่ากลับเป็นค่าเริ่มต้น เพื่อจะได้ไม่ต้องทำซ้ำ

    // ส่งค่าสถานะของหลอดขึ้น shadow
    bool_led_st = digitalRead(led);
    Serial.println("led is " + String(bool_led_st));
    sent_bool_led_st_to_netpie(1000);
  }



  Serial.print("./");  // เมื่อว่างก็ให้ทำการส่งข้อมูลนี้
}

void sent_bool_led_st_to_netpie(int delay_1)  // sent_bool_led_st_to_netpie(int_delay_1);
{
  String str_led_status = "{\"data\":{\"led status\":" + String(bool_led_st) + "}}";
  Serial.println(str_led_status);
  str_led_status.toCharArray(msg, (str_led_status.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(delay_1);
}

void sent_all_data_to_netpie(int delay_1)  // sent_all_data_to_netpie(int_delay_1);
{
  String str = "{\"data\" : {\"incoming\":" + String(incoming) + "}}";
  Serial.println(str);
  str.toCharArray(msg, (str.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(delay_1);
}
