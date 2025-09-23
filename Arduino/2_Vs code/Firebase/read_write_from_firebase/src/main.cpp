#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// --- กำหนดค่า Wi-Fi ของคุณ ---
#define WIFI_SSID "Test"
#define WIFI_PASSWORD "Test987654321"

// --- กำหนดค่า Firebase ของคุณ ---
#define FIREBASE_HOST "app-firebase-servo-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "ak7LKZApYAGTP5HBzClMHVHDRwyRmXP5wY6QTBPX"

// --- กำหนดพาธ Firebase สำหรับควบคุมและสถานะ ---
#define FIREBASE_BUTTON_PATH "/test/button1"          // พาธสำหรับคำสั่ง on/off เดิมจาก Serial
#define FIREBASE_PASS_PATH "/test/pass"               // พาธสำหรับคำสั่ง Relay พิเศษ (reset_10_sec, reset_1_minute, turn_off, non)
#define FIREBASE_ONLINE_STATUS_PATH "/test/last_seen" // พาธสำหรับอัปเดตสถานะออนไลน์ของ ESP8266

// --- สร้าง Object สำหรับ Firebase ---
FirebaseData firebaseData;
FirebaseConfig config;

// --- กำหนดขา Relay และสถานะ High/Low สำหรับ Relay ---
// กำหนดขา D4 ของ NodeMCU (ซึ่งคือ GPIO2) เป็นขาควบคุม Relay
const int relay = D4; // D4 = GPIO2 บน NodeMCU ESP8266

// กำหนดสถานะสำหรับเปิด/ปิด Relay
// จากโค้ดที่คุณให้มาล่าสุด: relay_high = LOW (OFF), relay_low = HIGH (ON)
// ซึ่งหมายถึง Relay Module ของคุณเป็นแบบ Active HIGH
const int relay_high = LOW; // สถานะที่ทำให้ Relay ปิด (OFF)
const int relay_low = HIGH; // สถานะที่ทำให้ Relay เปิด (ON)

// --- ตัวแปรสำหรับเก็บสถานะและการควบคุม ---
bool currentLedStateButton1 = false; // สำหรับสถานะจาก /test/button1

unsigned long ledOnStartTime = 0;           // เก็บเวลาที่ Relay ถูกเปิด
bool isLedTimerActive = false;              // สถานะของ Timer ว่ากำลังนับถอยหลังอยู่หรือไม่
long currentLedDuration = 0;                // ระยะเวลาที่ Relay จะติด (เช่น 10000 ms หรือ 60000 ms, 0 สำหรับ ON ตลอดไป)
unsigned long lastCountdownPrintTime = 0;   // เก็บเวลาที่พิมพ์นับถอยหลังล่าสุด
const long COUNTDOWN_PRINT_INTERVAL = 1000; // พิมพ์นับถอยหลังทุก 1 วินาที

// เพิ่มตัวแปรสถานะเพื่อจัดการการอัปเดต Firebase ที่ค้างอยู่ (สำหรับ /test/button1)
enum FirebaseUpdateState
{
  NO_UPDATE_NEEDED,
  PENDING_UPDATE_TO_FALSE_AFTER_TIMER_BUTTON1,   // อัปเดต button1 เป็น FALSE หลังจาก Timer ครบ
  PENDING_UPDATE_TO_FALSE_BY_SERIAL_OFF_BUTTON1, // อัปเดต button1 เป็น FALSE เพราะ Serial 'off'
  PENDING_UPDATE_TO_TRUE_BY_SERIAL_ON_BUTTON1,   // อัปเดต button1 เป็น TRUE เพราะ Serial 'on'
  WAITING_FOR_FIREBASE_CONFIRMATION_BUTTON1      // เพิ่มสถานะนี้เพื่อรอ Firebase ยืนยันว่าค่า button1 ถูกต้องแล้ว
};
FirebaseUpdateState firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;

// Enum ใหม่สำหรับสถานะ Relay จาก Firebase /test/pass
enum FirebaseRelayState
{                     // เปลี่ยนชื่อจาก FirebaseLedState เป็น FirebaseRelayState
  STATE_NON,          // ค่าว่าง ไม่ต้องทำอะไร
  STATE_ON_10_SEC,    // ON 10 วินาที
  STATE_ON_1_MINUTE,  // ON 1 นาที
  STATE_ON_INDEFINITE // ON ตลอดไป
};
FirebaseRelayState currentFirebaseRelayStatePass = STATE_NON; // สถานะที่ได้รับจาก /test/pass และกำลังทำงานอยู่
String pendingPassUpdateValue = "non";                        // ค่าที่ต้องการจะส่งไป update ที่ /test/pass เมื่อ Timer หมด หรือมีการ reconnect

// เพิ่มตัวแปรสำหรับสถานะจริงของ Relay (ว่าควรจะ ON หรือ OFF)
bool isRelayActuallyOn = false; // สถานะจริงของ Relay (true = ON, false = OFF)

// เพิ่มตัวแปรสำหรับควบคุมการเชื่อมต่อ Wi-Fi ด้วยคำสั่ง "test"
bool isTestModeActive = false;       // สถานะที่บอกว่ากำลังอยู่ในโหมดทดสอบด้วยคำสั่ง "test"
unsigned long testModeStartTime = 0; // เวลาที่เริ่มโหมดทดสอบ

// ตัวแปรสำหรับตรวจสอบสถานะการเชื่อมต่อ Wi-Fi
bool wasWifiConnectedLastLoop = false; // สถานะ Wi-Fi ใน loop ที่แล้ว

// เพิ่มตัวแปรสำหรับควบคุมการอัปเดตสถานะออนไลน์
unsigned long lastOnlineUpdate = 0;
const long ONLINE_UPDATE_INTERVAL = 30000; // อัปเดตทุก 30 วินาที (ปรับได้ตามต้องการ)

// Function Prototype (ประกาศฟังก์ชันล่วงหน้า)
void connectWifi();
void setRelayState(bool state);                              // เปลี่ยนชื่อฟังก์ชันเป็น setRelayState
void disconnectWifi();                                       // ฟังก์ชันสำหรับตัดการเชื่อมต่อ Wi-Fi
void handleSerialInput();                                    // ฟังก์ชันจัดการ Serial Input
void handleRelayTimer();                                     // เปลี่ยนชื่อฟังก์ชันเป็น handleRelayTimer
void handleFirebaseOperations();                             // ฟังก์ชันจัดการ Firebase Read/Write
void activateRelayFromFirebase(FirebaseRelayState newState); // เปลี่ยนชื่อฟังก์ชันเป็น activateRelayFromFirebase
void updatePassToFirebase(String value);                     // ฟังก์ชันใหม่สำหรับอัปเดต /test/pass

void setup()
{
  Serial.begin(115200);

  pinMode(relay, OUTPUT); // กำหนดขา Relay เป็น OUTPUT
  setRelayState(false);   // ตั้งค่าเริ่มต้นให้ Relay ปิด

  connectWifi(); // เชื่อมต่อ Wi-Fi
  wasWifiConnectedLastLoop = (WiFi.status() == WL_CONNECTED);

  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, NULL);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase initialized. Ready to read data from Firebase.");
  Serial.println("Waiting for changes at " FIREBASE_BUTTON_PATH " or " FIREBASE_PASS_PATH " ...");
  Serial.println("Type 'on' in Serial Monitor to turn Relay ON.");
  Serial.println("Type 'off' in Serial Monitor to turn Relay OFF.");
  Serial.println("Type 'disconnect' in Serial Monitor to disconnect WiFi.");
  Serial.println("Type 'reconnect' in Serial Monitor to force reconnect WiFi.");
  Serial.println("Type 'test' in Serial Monitor to turn Relay ON for 10s and disconnect WiFi.");

  // อ่านค่าเริ่มต้นจาก Firebase /test/button1 หนึ่งครั้งเมื่อเริ่มต้น
  if (Firebase.getBool(firebaseData, FIREBASE_BUTTON_PATH))
  {
    if (firebaseData.dataType() == "boolean")
    {
      currentLedStateButton1 = firebaseData.boolData();
      Serial.print("Initial value of " FIREBASE_BUTTON_PATH ": ");
      Serial.println(currentLedStateButton1 ? "TRUE" : "FALSE");
      if (currentLedStateButton1)
      {
        setRelayState(true);
        ledOnStartTime = millis();
        isLedTimerActive = true;
        currentLedDuration = 10000; // Hardcode 10s for button1
        lastCountdownPrintTime = millis();
        Serial.println("Relay ON (from initial button1 value). Timer started for 10 seconds.");
      }
      else
      {
        setRelayState(false);
      }
    }
    else
    {
      Serial.print("Initial value of " FIREBASE_BUTTON_PATH " is not boolean. Data type: ");
      Serial.println(firebaseData.dataType());
    }
  }
  else
  {
    Serial.println("Failed to read initial value of " FIREBASE_BUTTON_PATH ".");
  }

  // อ่านค่าเริ่มต้นจาก Firebase /test/pass หนึ่งครั้งเมื่อเริ่มต้น
  if (Firebase.getString(firebaseData, FIREBASE_PASS_PATH))
  {
    if (firebaseData.dataType() == "string")
    {
      String passCommand = firebaseData.stringData();
      Serial.print("Initial value of " FIREBASE_PASS_PATH ": ");
      Serial.println(passCommand);
      if (passCommand == "reset_10_sec")
      {
        currentFirebaseRelayStatePass = STATE_ON_10_SEC;
        activateRelayFromFirebase(STATE_ON_10_SEC);
      }
      else if (passCommand == "reset_1_minute")
      {
        currentFirebaseRelayStatePass = STATE_ON_1_MINUTE;
        activateRelayFromFirebase(STATE_ON_1_MINUTE);
      }
      else if (passCommand == "turn_off")
      {
        currentFirebaseRelayStatePass = STATE_ON_INDEFINITE;
        activateRelayFromFirebase(STATE_ON_INDEFINITE);
      }
      else
      { // หากเป็นค่าอื่น เช่น "non" หรือ "off" หรือค่าว่าง
        currentFirebaseRelayStatePass = STATE_NON;
        activateRelayFromFirebase(STATE_NON);
      }
    }
    else
    {
      Serial.print("Initial value of " FIREBASE_PASS_PATH " is not string. Data type: ");
      Serial.println(firebaseData.dataType());
      currentFirebaseRelayStatePass = STATE_NON;
    }
  }
  else
  {
    Serial.println("Failed to read initial value of " FIREBASE_PASS_PATH ".");
    currentFirebaseRelayStatePass = STATE_NON;
  }
}

void loop()
{
  handleSerialInput();
  handleRelayTimer();
  handleFirebaseOperations();

  // ตรวจสอบการเชื่อมต่อ Wi-Fi ในทุก Loop เพื่อจัดการ update /test/pass เมื่อกลับมาเชื่อมต่อ
  bool currentWifiConnected = (WiFi.status() == WL_CONNECTED);
  if (!wasWifiConnectedLastLoop && currentWifiConnected)
  {
    Serial.println("WiFi reconnected!");
    // เมื่อ Wi-Fi กลับมาเชื่อมต่อ ให้พยายาม update /test/pass ทันทีหากมีค่าค้างอยู่
    updatePassToFirebase(pendingPassUpdateValue);

    // อัปเดตสถานะออนไลน์เมื่อ WiFi กลับมาเชื่อมต่อ
    if (Firebase.setTimestamp(firebaseData, FIREBASE_ONLINE_STATUS_PATH))
    {
      Serial.println("Updated online status timestamp on reconnect.");
    }
    else
    {
      Serial.println("Failed to update online status timestamp on reconnect: " + firebaseData.errorReason());
    }
  }
  wasWifiConnectedLastLoop = currentWifiConnected;

  // อัปเดตสถานะออนไลน์เป็นประจำเมื่อเชื่อมต่อ Firebase ได้
  if (WiFi.status() == WL_CONNECTED && Firebase.ready())
  {
    if (millis() - lastOnlineUpdate >= ONLINE_UPDATE_INTERVAL)
    {
      if (Firebase.setTimestamp(firebaseData, FIREBASE_ONLINE_STATUS_PATH))
      {
        Serial.println("Updated online status timestamp.");
        lastOnlineUpdate = millis();
      }
      else
      {
        Serial.println("Failed to update online status timestamp: " + firebaseData.errorReason());
      }
    }
  }

  delay(10);
}

void handleSerialInput()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "on")
    {
      Serial.println("Received 'on' command. Turning Relay ON and updating Firebase.");
      currentLedStateButton1 = true;
      activateRelayFromFirebase(STATE_ON_10_SEC);
      firebaseUpdateStateButton1 = PENDING_UPDATE_TO_TRUE_BY_SERIAL_ON_BUTTON1;

      // เมื่อสั่ง 'on' หรือ 'off' จาก Serial ให้ถือว่ายกเลิกการควบคุมจาก /test/pass
      // โดยการส่ง "non" กลับไปที่ /test/pass (เพื่อเคลียร์ค่า) และรีเซ็ตสถานะภายใน
      pendingPassUpdateValue = "non";
      updatePassToFirebase(pendingPassUpdateValue);
      currentFirebaseRelayStatePass = STATE_NON;
    }
    else if (command == "off")
    {
      Serial.println("Received 'off' command. Turning Relay OFF and updating Firebase.");
      activateRelayFromFirebase(STATE_NON);
      currentLedStateButton1 = false;
      isTestModeActive = false;
      firebaseUpdateStateButton1 = PENDING_UPDATE_TO_FALSE_BY_SERIAL_OFF_BUTTON1;

      // เมื่อสั่ง 'on' หรือ 'off' จาก Serial ให้ถือว่ายกเลิกการควบคุมจาก /test/pass
      pendingPassUpdateValue = "non";
      updatePassToFirebase(pendingPassUpdateValue);
      currentFirebaseRelayStatePass = STATE_NON;
      Serial.println("Relay is OFF. Timer stopped.");
    }
    else if (command == "disconnect")
    {
      Serial.println("Received 'disconnect' command.");
      disconnectWifi();
      isTestModeActive = false;
      firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;
    }
    else if (command == "reconnect")
    {
      Serial.println("Received 'reconnect' command. Attempting to reconnect WiFi...");
      connectWifi();
      isTestModeActive = false;
      firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;
    }
    else if (command == "test")
    {
      Serial.println("Received 'test' command. Starting 10-second Relay ON and WiFi disconnect.");
      // คำสั่ง test จะ override ทุกอย่าง
      setRelayState(true);
      ledOnStartTime = millis();
      isLedTimerActive = true;
      currentLedDuration = 10000;
      lastCountdownPrintTime = millis();
      disconnectWifi();
      isTestModeActive = true;
      testModeStartTime = millis();
      firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;

      // เมื่อสั่ง 'test' ให้ถือว่ายกเลิกการควบคุมจาก /test/pass
      pendingPassUpdateValue = "non";
      updatePassToFirebase(pendingPassUpdateValue);
      currentFirebaseRelayStatePass = STATE_NON;
    }
    else
    {
      Serial.print("Unknown command: ");
      Serial.println(command);
    }
  }
}

void handleRelayTimer()
{
  if (isLedTimerActive)
  {
    // ถ้าเป็นโหมด ON ค้าง (STATE_ON_INDEFINITE) ก็ไม่ต้องนับถอยหลัง
    if (currentLedDuration == 0)
    {
      if (millis() - lastCountdownPrintTime >= 5000)
      { // พิมพ์ทุก 5 วินาที
        lastCountdownPrintTime = millis();
        Serial.println("Relay ON: Indefinite mode active (from /test/pass: turn_off).");
      }
      return;
    }

    long elapsedTime = millis() - ledOnStartTime;
    long remainingTime = currentLedDuration - elapsedTime;

    // พิมพ์นับถอยหลังทุก 1 วินาที
    if (remainingTime > 0 && (millis() - lastCountdownPrintTime >= COUNTDOWN_PRINT_INTERVAL))
    {
      lastCountdownPrintTime = millis();
      Serial.print("Time remaining (Relay ON): ");
      Serial.println(remainingTime / 1000 + 1);
    }

    // ถ้าเวลาครบแล้ว
    if (elapsedTime >= currentLedDuration)
    {
      setRelayState(false);
      isLedTimerActive = false;
      currentLedStateButton1 = false;
      currentFirebaseRelayStatePass = STATE_NON;

      Serial.print(currentLedDuration / 1000);
      Serial.println(" seconds passed. Relay OFF.");

      // เมื่อ Relay ดับ ให้เข้าสู่สถานะที่ต้องอัปเดต Firebase button1 เป็น FALSE
      firebaseUpdateStateButton1 = PENDING_UPDATE_TO_FALSE_AFTER_TIMER_BUTTON1;

      if (isTestModeActive)
      { // ถ้าอยู่ในโหมดทดสอบ
        Serial.println("Test mode complete. Initiating WiFi reconnect and Firebase update to FALSE.");
        connectWifi();            // เชื่อมต่อ Wi-Fi ใหม่ (เผื่อกรณีถูกตัดจากการสั่ง 'test')
        isTestModeActive = false; // ปิดโหมดทดสอบ
      }

      // เมื่อ Timer หมด ให้ตั้งค่า pendingPassUpdateValue เป็น "non" และพยายาม update
      pendingPassUpdateValue = "non";
      updatePassToFirebase(pendingPassUpdateValue);
    }
  }
}

void handleFirebaseOperations()
{
  // ตรวจสอบสถานะ Wi-Fi ก่อน
  if (WiFi.status() != WL_CONNECTED)
  {
    if (!isTestModeActive || (isTestModeActive && (millis() - testModeStartTime > 10000 + 500)))
    {
      Serial.println("WiFi Disconnected. Waiting for reconnection...");
    }
    return; // ออกจากฟังก์ชันนี้ ถ้าไม่มี WiFi จะทำ Firebase ไม่ได้
  }

  // ถ้า WiFi Connected แต่ Firebase ยังไม่พร้อม
  if (!Firebase.ready())
  {
    Serial.println("Firebase not ready. Please wait for Firebase connection.");
    return;
  }

  // ส่วนของการจัดการการอัปเดต Firebase button1 เป็น FALSE/TRUE
  bool shouldSendUpdateButton1 = false;
  bool valueToSendButton1 = false; // ค่าที่จะส่งไป Firebase (true/false)
  String targetPathButton1 = "";   // พาธที่จะส่งไป

  if (firebaseUpdateStateButton1 == PENDING_UPDATE_TO_FALSE_AFTER_TIMER_BUTTON1 ||
      firebaseUpdateStateButton1 == PENDING_UPDATE_TO_FALSE_BY_SERIAL_OFF_BUTTON1)
  {
    shouldSendUpdateButton1 = true;
    valueToSendButton1 = false;
    targetPathButton1 = FIREBASE_BUTTON_PATH; // อัปเดต button1 เป็น false
  }
  else if (firebaseUpdateStateButton1 == PENDING_UPDATE_TO_TRUE_BY_SERIAL_ON_BUTTON1)
  {
    shouldSendUpdateButton1 = true;
    valueToSendButton1 = true;
    targetPathButton1 = FIREBASE_BUTTON_PATH; // อัปเดต button1 เป็น true
  }

  if (shouldSendUpdateButton1)
  {
    Serial.print("Attempting to update Firebase '" + targetPathButton1 + "' to " + (valueToSendButton1 ? "TRUE" : "FALSE") + "...");
    if (Firebase.setBool(firebaseData, targetPathButton1, valueToSendButton1))
    {
      Serial.println("Firebase '" + targetPathButton1 + "' updated successfully. Now confirming...");
      firebaseUpdateStateButton1 = WAITING_FOR_FIREBASE_CONFIRMATION_BUTTON1; // ไปรอการยืนยัน
    }
    else
    {
      Serial.println("Error updating Firebase '" + targetPathButton1 + "': " + firebaseData.errorReason());
      // จะลองใหม่ใน loop ถัดไป
    }
  }

  // โค้ดส่วนนี้จะถูกข้ามเมื่ออยู่ในโหมดทดสอบ เพื่อให้คำสั่ง test ควบคุมอย่างเดียว
  if (!isTestModeActive)
  {
    // อ่านค่าจาก /test/button1
    if (Firebase.getBool(firebaseData, FIREBASE_BUTTON_PATH))
    {
      if (firebaseData.dataType() == "boolean")
      {
        bool firebaseValue = firebaseData.boolData();

        // ตรวจสอบว่าค่าจาก Firebase /test/button1 เปลี่ยนไป หรือกำลังรอการยืนยันจาก /test/button1
        // และยังไม่ได้ถูกควบคุมโดยคำสั่งจาก /test/pass
        if ((firebaseValue != currentLedStateButton1 ||
             (firebaseUpdateStateButton1 == WAITING_FOR_FIREBASE_CONFIRMATION_BUTTON1 && firebaseData.dataPath() == FIREBASE_BUTTON_PATH)) &&
            currentFirebaseRelayStatePass == STATE_NON)
        {

          Serial.print("Firebase '" FIREBASE_BUTTON_PATH "' changed to: ");
          Serial.println(firebaseValue ? "TRUE" : "FALSE");

          // หาก Firebase เป็น FALSE และเรากำลังรอการยืนยัน (สำหรับ button1)
          if (!firebaseValue && (firebaseUpdateStateButton1 == WAITING_FOR_FIREBASE_CONFIRMATION_BUTTON1 ||
                                 firebaseUpdateStateButton1 == PENDING_UPDATE_TO_FALSE_AFTER_TIMER_BUTTON1 ||
                                 firebaseUpdateStateButton1 == PENDING_UPDATE_TO_FALSE_BY_SERIAL_OFF_BUTTON1))
          {
            Serial.println("Firebase confirmation received: '" FIREBASE_BUTTON_PATH "' is now FALSE.");
            firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;
            currentLedStateButton1 = firebaseValue;
            activateRelayFromFirebase(STATE_NON);
            currentFirebaseRelayStatePass = STATE_NON;
          }
          // หาก Firebase เป็น TRUE และเรากำลังรอการยืนยัน (สำหรับ button1)
          else if (firebaseValue && (firebaseUpdateStateButton1 == WAITING_FOR_FIREBASE_CONFIRMATION_BUTTON1 ||
                                     firebaseUpdateStateButton1 == PENDING_UPDATE_TO_TRUE_BY_SERIAL_ON_BUTTON1))
          {
            Serial.println("Firebase confirmation received: '" FIREBASE_BUTTON_PATH "' is now TRUE.");
            firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;
            currentLedStateButton1 = firebaseValue;
            activateRelayFromFirebase(STATE_ON_10_SEC);
          }
          // กรณีที่ Firebase /test/button1 ส่งค่ามาและมันแตกต่างจาก currentLedStateButton1 ที่บอร์ดรับรู้
          // และเราไม่ได้อยู่ในสถานะรอการยืนยัน (หมายถึง Firebase สั่งมาเอง)
          else if (firebaseValue != currentLedStateButton1)
          {
            currentLedStateButton1 = firebaseValue;
            if (currentLedStateButton1)
            {
              activateRelayFromFirebase(STATE_ON_10_SEC);
            }
            else
            {
              activateRelayFromFirebase(STATE_NON);
              firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;
            }
          }
        }
      }
      else
      {
        Serial.print("Firebase '" FIREBASE_BUTTON_PATH "' is not a boolean type. Data type: ");
        Serial.println(firebaseData.dataType());
      }
    }
    else
    {
      // Serial.println("Failed to read '" FIREBASE_BUTTON_PATH "': " + firebaseData.errorReason());
    }

    // --- ส่วนการอ่านค่าจาก /test/pass (คำสั่ง Relay พิเศษ) ---
    if (Firebase.getString(firebaseData, FIREBASE_PASS_PATH))
    {
      if (firebaseData.dataType() == "string")
      {
        String passCommand = firebaseData.stringData();

        FirebaseRelayState newFirebaseRelayState = STATE_NON;

        if (passCommand == "reset_10_sec")
        {
          newFirebaseRelayState = STATE_ON_10_SEC;
        }
        else if (passCommand == "reset_1_minute")
        {
          newFirebaseRelayState = STATE_ON_1_MINUTE;
        }
        else if (passCommand == "turn_off")
        {
          newFirebaseRelayState = STATE_ON_INDEFINITE;
        }
        else if (passCommand == "non" || passCommand == "" || passCommand == "off")
        {
          newFirebaseRelayState = STATE_NON;
        }

        // *** เงื่อนไขการรับคำสั่งจาก /test/pass ที่สำคัญ ***
        // จะรับคำสั่งและสั่ง Relay ก็ต่อเมื่อ:
        // 1. คำสั่งใหม่แตกต่างจากสถานะปัจจุบันที่บอร์ดกำลังทำงานอยู่ (currentFirebaseRelayStatePass)
        //    (เช่น ถ้ากำลัง ON 10s อยู่ แล้วได้รับ reset_10_sec ซ้ำ ก็จะไม่ทำอะไร)
        // 2. หรือเป็นคำสั่ง STATE_NON (ปิด) และ Relay กำลังเปิดอยู่ (เพื่อบังคับปิด)
        // 3. หรือเป็นคำสั่ง ON ชนิดมี Timer (10s, 1min) และ Relay กำลังเปิดแบบ indefinite (เพื่อรีเซ็ต Timer)
        if (newFirebaseRelayState != currentFirebaseRelayStatePass ||
            (newFirebaseRelayState == STATE_NON && isRelayActuallyOn) ||
            ((newFirebaseRelayState == STATE_ON_10_SEC || newFirebaseRelayState == STATE_ON_1_MINUTE) && currentFirebaseRelayStatePass == STATE_ON_INDEFINITE))
        {

          // หากคำสั่งที่ได้รับมาเป็น "non" หรือ "off"
          // และบอร์ดกำลังอยู่ในสถานะที่ Relay เปิดอยู่ด้วย Timer หรือ indefinite
          // ให้ *ไม่สนใจ* คำสั่ง "non"/"off" นี้ และรอให้ Timer หมด หรือมีคำสั่ง ON ใหม่เข้ามา
          if ((newFirebaseRelayState == STATE_NON) && (isLedTimerActive || currentFirebaseRelayStatePass == STATE_ON_INDEFINITE))
          {
            Serial.println("Received 'non' command, but Relay is currently active. Ignoring for now.");
            return;
          }

          currentFirebaseRelayStatePass = newFirebaseRelayState;
          Serial.print("Firebase '" FIREBASE_PASS_PATH "' changed to: ");
          Serial.println(passCommand);

          activateRelayFromFirebase(currentFirebaseRelayStatePass);

          // รีเซ็ตสถานะของ /test/button1 และอัปเดต Firebase ให้เป็น FALSE เพื่อป้องกันความขัดแย้ง
          currentLedStateButton1 = false;
          firebaseUpdateStateButton1 = NO_UPDATE_NEEDED;
          if (WiFi.status() == WL_CONNECTED && Firebase.ready())
          {
            // อัปเดต button1 ให้เป็น FALSE เสมอเมื่อมีการสั่งงานจาก /test/pass
            if (Firebase.setBool(firebaseData, FIREBASE_BUTTON_PATH, false))
            {
              Serial.println("Updated " FIREBASE_BUTTON_PATH " to FALSE due to " FIREBASE_PASS_PATH " command.");
            }
            else
            {
              Serial.println("Failed to update " FIREBASE_BUTTON_PATH " to FALSE.");
            }
          }

          // ตั้งค่า pendingPassUpdateValue เพื่อรอ update /test/pass เป็น "non" หลังจากประมวลผลคำสั่ง
          if (currentFirebaseRelayStatePass != STATE_NON)
          {
            pendingPassUpdateValue = "non"; // เตรียมส่ง "non" กลับไปเมื่อทำงานเสร็จ
          }
          else
          {                                 // ถ้าคำสั่งที่เข้ามาเป็น non อยู่แล้ว
            pendingPassUpdateValue = "non"; // ยืนยันว่าค่าที่ต้องการคือ non
          }
          updatePassToFirebase(pendingPassUpdateValue);
        }
        else if (newFirebaseRelayState == currentFirebaseRelayStatePass)
        {
          // หากได้รับคำสั่งซ้ำ และเป็นสถานะที่กำลังทำงานอยู่ (เช่น reset_10_sec ซ้ำในขณะที่กำลังนับถอยหลัง 10s)
          // เราจะยังคงทำอะไรตามปกติ แต่ไม่ต้องไป activateRelayFromFirebase ซ้ำ
        }
      }
      else
      {
        Serial.print("Firebase '" FIREBASE_PASS_PATH "' is not a string type. Data type: ");
        Serial.println(firebaseData.dataType());
      }
    }
    else
    {
      // Serial.println("Failed to read '" FIREBASE_PASS_PATH "': " + firebaseData.errorReason());
    }
  }
}

// ฟังก์ชันใหม่สำหรับจัดการ Relay ตามคำสั่งจาก Firebase /test/pass หรือ Serial
void activateRelayFromFirebase(FirebaseRelayState newState)
{
  if (newState == STATE_NON)
  { // ถ้าเป็นคำสั่ง NON (ปิด) โดยตรง
    setRelayState(false);
    isLedTimerActive = false; // หยุด Timer
    currentLedDuration = 0;
    Serial.println("Deactivating Relay: OFF (from STATE_NON).");
  }
  else
  { // ถ้าเป็นคำสั่ง ON ทุกชนิด
    setRelayState(true);
    ledOnStartTime = millis();         // เริ่มจับเวลาเสมอเมื่อ ON
    lastCountdownPrintTime = millis(); // รีเซ็ตการพิมพ์นับถอยหลังเสมอเมื่อ ON

    if (newState == STATE_ON_10_SEC)
    {
      currentLedDuration = 10000; // 10 วินาที
      isLedTimerActive = true;
      Serial.println("Activating Relay: ON for 10 seconds (from /test/pass: reset_10_sec).");
      Serial.print("Time remaining (Relay ON): ");
      Serial.println(currentLedDuration / 1000);
    }
    else if (newState == STATE_ON_1_MINUTE)
    {
      currentLedDuration = 60000; // 1 นาที
      isLedTimerActive = true;
      Serial.println("Activating Relay: ON for 1 minute (from /test/pass: reset_1_minute).");
      Serial.print("Time remaining (Relay ON): ");
      Serial.println(currentLedDuration / 1000);
    }
    else if (newState == STATE_ON_INDEFINITE)
    {
      currentLedDuration = 0;  // 0 หมายถึง ON ตลอดไป ไม่ต้องนับถอยหลัง
      isLedTimerActive = true; // ยังคงให้ Timer Active แต่ไม่นับถอยหลังเพื่อคงสถานะ ON
      Serial.println("Activating Relay: ON indefinitely (from /test/pass: turn_off).");
    }
  }
}

// ฟังก์ชันใหม่สำหรับอัปเดต /test/pass
void updatePassToFirebase(String value)
{
  if (WiFi.status() == WL_CONNECTED && Firebase.ready())
  {
    // ตรวจสอบค่าปัจจุบันใน Firebase ก่อนที่จะเขียนทับ
    String currentFirebasePassValue = "";
    if (Firebase.getString(firebaseData, FIREBASE_PASS_PATH))
    {
      if (firebaseData.dataType() == "string")
      {
        currentFirebasePassValue = firebaseData.stringData();
      }
    }

    if (currentFirebasePassValue != value)
    { // ถ้าค่าไม่ตรงกับที่ต้องการจะเขียน
      Serial.print("Attempting to update " FIREBASE_PASS_PATH " to '" + value + "'...");
      if (Firebase.setString(firebaseData, FIREBASE_PASS_PATH, value))
      {
        Serial.println("Successfully updated " FIREBASE_PASS_PATH " to '" + value + "'.");
        pendingPassUpdateValue = value; // ยืนยันว่าค่าที่ต้องการถูกส่งแล้ว
      }
      else
      {
        Serial.println("Failed to update " FIREBASE_PASS_PATH " to '" + value + "': " + firebaseData.errorReason());
      }
    }
    else
    {
      Serial.println(FIREBASE_PASS_PATH " is already '" + value + "'. No update needed.");
      pendingPassUpdateValue = value; // ยืนยันว่าค่าที่ต้องการถูกส่งแล้ว
    }
  }
  else
  {
    Serial.println("WiFi not connected or Firebase not ready. Cannot update " FIREBASE_PASS_PATH " to '" + value + "'. Will retry.");
    pendingPassUpdateValue = value; // ยังคงต้องการอัปเดตเมื่อเชื่อมต่อได้
  }
}

void connectWifi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Already connected to WiFi.");
    Serial.print("Current IP Address: ");
    Serial.println(WiFi.localIP());
    return;
  }
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 30000)
    {
      Serial.println("\nWiFi connection timed out.");
      return;
    }
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setRelayState(bool state)
{
  if (state)
  {
    digitalWrite(relay, relay_low); // เปิด Relay
    isRelayActuallyOn = true;
  }
  else
  {
    digitalWrite(relay, relay_high); // ปิด Relay
    isRelayActuallyOn = false;
  }
}

void disconnectWifi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
    Serial.println("WiFi disconnected manually.");
  }
  else
  {
    Serial.println("WiFi is already disconnected.");
  }
}