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


/*
#define ldr A0
float ADC_value = 0.0048828125;
char msg[150];
*/
#define led LED_BUILTIN
int c1 = 0;
char msg[150];





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

  pinMode(led, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  c1++;

  /*
  String data = "{\"data\": {\"light\":" + String(lux) + "}}";
  Serial.println(data);
  data.toCharArray(msg, (data.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(5000);
*/

  int temp = 39;
  int humidity = 50;

          //     {"data" : {"count" : c1}}"
  //String data = "{ \"data\" : { \"count\" :" + String(c1)+ "}}";

          //     {"data" : {"count" : c1 , "temp" : temp , "humuduty" : humidity }}"
  String data = "{\"data\" : {\"count\" :" +String(c1)+ ", \"temp\" :" +String(temp)+ ", \"humuduty\" :" +String(humidity)+ "}}";


  Serial.println(data);
  data.toCharArray(msg, (data.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(2000);
}
