#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ==== Wi-Fi Configuration ====
#define WIFI_SSID "Test"              // เปลี่ยนเป็นชื่อ Wi-Fi ของคุณ
#define WIFI_PASSWORD "Test987654321" // เปลี่ยนเป็นรหัสผ่าน Wi-F i ของคุณ

// ==== PCA9685 ====
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// ==== Web Server ====
ESP8266WebServer server(80);
 
// ==== Servo Control Variables ====
int servoChannel = 0;  // กำหนดช่อง PCA9685 ที่ใช้สำหรับเซอร์โวของคุณ (ช่องที่ 0)
int servoMin = 320;    // ค่าพัลส์ต่ำสุดสำหรับเซอร์โวของคุณ
int servoMax = 860;    // ค่าพัลส์สูงสุดสำหรับเซอร์โวของคุณ
int currentAngle = 90; // ตำแหน่งเริ่มต้นของเซอร์โว

// Function to handle incoming HTTP requests
void handleServo()
{
  if (server.hasArg("angle"))
  { // Check for the 'angle' argument
    String angleStr = server.arg("angle");
    int receivedAngle = angleStr.toInt();

    // Clamp the angle to a valid range
    receivedAngle = max(0, min(180, receivedAngle));

    // Convert angle to pulse value and command the PCA9685
    int pulse = map(receivedAngle, 0, 180, servoMin, servoMax);
    pwm.setPWM(servoChannel, 0, pulse);

    Serial.print("Received angle: ");
    Serial.println(receivedAngle);
  }

  server.send(200, "text/plain", "OK");
}

void setup()
{
  Serial.begin(115200);

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // ตั้งค่า PCA9685
  Wire.begin(); // เริ่มต้น I2C
  pwm.begin();
  pwm.setPWMFreq(60);

  // ตั้งค่า Web Server
  server.on("/servo_control", handleServo);
  server.begin();
  Serial.println("HTTP server started");

  // ตั้งค่าตำแหน่งเริ่มต้นของเซอร์โว
  int initialPulse = map(currentAngle, 0, 180, servoMin, servoMax);
  pwm.setPWM(servoChannel, 0, initialPulse);
}

void loop()
{
  server.handleClient();
}