#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ==== Wi-Fi Configuration ====
#define WIFI_SSID "Test"              // Change to your Wi-Fi name
#define WIFI_PASSWORD "Test987654321" // Change to your Wi-Fi password

// ==== PCA9685 ====
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// ==== Web Server ====
ESP8266WebServer server(80);
 
// ==== Servo Control Variables ====
int servoChannel = 0;   // PCA9685 channel for your servo (channel 0)
int servoMin = 100;     // Minimum pulse value for your servo
int servoMax = 630;     // Maximum pulse value for your servo
int currentAngle = 90;  // Initial servo position

// Add variables for operation mode
String mode = "auto";  // 'auto' for web control, 'manual' for Serial Monitor control
int manualAngle = 90; // Angle for manual mode

// Function to print the current servo settings
void printCurrentSettings() {
  Serial.println("-------------------------");
  Serial.println("Current Servo Settings:");
  Serial.print("  Mode: ");
  Serial.println(mode);
  Serial.print("  Servo Min Pulse: ");
  Serial.println(servoMin);
  Serial.print("  Servo Max Pulse: ");
  Serial.println(servoMax);
  if (mode == "manual") {
    Serial.print("  Manual Angle: ");
    Serial.println(manualAngle);
  }
  Serial.println("-------------------------");
}

// Function to print all available commands
void printHelp() {
  Serial.println("-------------------------");
  Serial.println("Available Commands:");
  Serial.println("  ? or help: Display this help message and current settings.");
  Serial.println("  mode=auto: Switch to automatic mode (control via web server).");
  Serial.println("  mode=manual: Switch to manual mode (control via Serial Monitor).");
  Serial.println("  servoMin=[value]: Set the minimum pulse value (e.g., servoMin=250).");
  Serial.println("  servoMax=[value]: Set the maximum pulse value (e.g., servoMax=950).");
  Serial.println("  manual_angle=[0-180]: Set the servo angle (only in manual mode).");
  Serial.println("-------------------------");
}

// Function to handle incoming HTTP requests (only in auto mode)
void handleServo()
{
  // Only execute in auto mode
  if (mode == "auto" && server.hasArg("angle"))
  {
    String angleStr = server.arg("angle");
    int receivedAngle = angleStr.toInt();

    // Clamp the angle to a valid range
    receivedAngle = max(0, min(180, receivedAngle));

    // Convert angle to pulse value and command the PCA9685
    int pulse = map(receivedAngle, 0, 180, servoMin, servoMax);
    pwm.setPWM(servoChannel, 0, pulse);

    Serial.print("Received auto angle: ");
    Serial.println(receivedAngle);
  }

  server.send(200, "text/plain", "OK");
}

void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup PCA9685
  Wire.begin(); // Start I2C
  pwm.begin();
  pwm.setPWMFreq(60);

  // Setup Web Server
  server.on("/servo_control", handleServo);
  server.begin();
  Serial.println("HTTP server started");

  // Set initial servo position
  int initialPulse = map(currentAngle, 0, 180, servoMin, servoMax);
  pwm.setPWM(servoChannel, 0, initialPulse);
  Serial.println("Initial servo position set to 90 degrees.");
  printHelp();
}

void loop()
{
  server.handleClient();

  // Check for data from Serial Monitor
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "?" || command == "help") {
      printHelp();
      printCurrentSettings();
    }
    else if (command.startsWith("mode="))
    {
      String newMode = command.substring(5);
      if (newMode == "auto" || newMode == "manual")
      {
        mode = newMode;
        Serial.print("Mode switched to: ");
        Serial.println(mode);
      }
      else
      {
        Serial.println("Invalid mode command. Use 'auto' or 'manual'.");
      }
    }
    else if (command.startsWith("servoMin="))
    {
      servoMin = command.substring(9).toInt();
      Serial.print("servoMin updated to: ");
      Serial.println(servoMin);
    }
    else if (command.startsWith("servoMax="))
    {
      servoMax = command.substring(9).toInt();
      Serial.print("servoMax updated to: ");
      Serial.println(servoMax);
    }
    else if (command.startsWith("manual_angle="))
    {
      int newAngle = command.substring(13).toInt();
      if (newAngle >= 0 && newAngle <= 180)
      {
        manualAngle = newAngle;
        if (mode == "manual")
        {
          int pulse = map(manualAngle, 0, 180, servoMin, servoMax);
          pwm.setPWM(servoChannel, 0, pulse);
          Serial.print("Set manual angle to: ");
          Serial.println(manualAngle);
        }
        else
        {
          Serial.println("Switch to manual mode first with 'mode=manual'");
        }
      }
      else
      {
        Serial.println("Invalid angle. Please use a value between 0 and 180.");
      }
    }
    else
    {
      Serial.println("Invalid command. Type 'help' for a list of commands.");
    }
  }

  // In manual mode, set the servo based on the latest manualAngle value
  if (mode == "manual")
  {
    int pulse = map(manualAngle, 0, 180, servoMin, servoMax);
    pwm.setPWM(servoChannel, 0, pulse);
  }
}
