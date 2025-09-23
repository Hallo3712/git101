#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Test";
const char* password = "Test987654321";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "04a9bf82-326a-4b95-a6e7-260d3e87e7cc";
const char* mqtt_username = "Wq7BxGDMXpa7up7Wg2WgnyCXHVBVeJY3";
const char* mqtt_password = "EgX9tJwgv8aLVVvqB31CHuN5Aw2bMXeD";
WiFiClient espClient;
PubSubClient client(espClient);

#define led LED_BUILTIN

String data;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection....");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/test2");
    } else {
      Serial.print("failed, rc =^ prime prime");
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
  data = message;
}

void setup() {
  Serial.begin(115200);
  Serial.println(0);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
  }

  pinMode(led, OUTPUT);
  digitalWrite(led,LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  client.publish("@msg/test", "Hello world");
  Serial.println("Published suscess");
  delay(200);

  Serial.println("data is " + data);
  if(data == "1"){
    digitalWrite(led,!digitalRead(led));
    Serial.println("led is on");
    data = 0;
  }
}
