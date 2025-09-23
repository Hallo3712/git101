#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "test";
const char* password = "987654321";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "a3ca77c6-1a22-4e9e-bb9e-89a0dce52060";
const char* mqtt_username = "s8XHdF2YuTY9FYikwhRZebTymfqa8EV6";
const char* mqtt_password = "LRhAx6z9bR94UycpssSxWnYR3WUPFNek";

WiFiClient espClient;
PubSubClient client(espClient);

int mill1 = 0;

int c1 = 0;
int c2 = 0;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/test2");
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
  Serial.println(message);
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
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (millis() - mill1 >= 1000) {
    mill1 = millis();
    client.publish("@msg/test1", "what");
    Serial.print("done ");
    c1++;
    Serial.println(String("_") + int(c1) + String("_"));
  }

}
