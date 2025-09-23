// กำหนดขาของ LED_BUILTIN ซึ่งปกติจะอยู่ที่ขา 13 บน Arduino Uno
const int ledPin = LED_BUILTIN;

// ตัวแปรสำหรับเก็บสถานะปัจจุบันของ LED
bool ledOn = false;

// ตัวแปรสำหรับเก็บสถานะการกระพริบ
bool isBlinking = false;
long previousMillis = 0;
const long blinkInterval = 500; // ตั้งค่าช่วงเวลาการกระพริบ 500 มิลลิวินาที (0.5 วินาที)

// ฟังก์ชัน setup() จะทำงานเพียงครั้งเดียวเมื่อเปิดบอร์ด
void setup() {
  // ตั้งค่าขา LED เป็น OUTPUT
  pinMode(ledPin, OUTPUT);
  // เริ่มต้น Serial Communication ที่ความเร็ว 9600
  Serial.begin(9600);
  Serial.println("Ready to receive commands: 'เปิด', 'ปิด', 'กระพริบ'");
}

// ฟังก์ชัน loop() จะทำงานซ้ำๆ อย่างต่อเนื่อง
void loop() {
  // ตรวจสอบว่ามีข้อมูลเข้ามาทาง Serial Monitor หรือไม่
  if (Serial.available()) {
    // อ่านคำสั่งที่ส่งมาและแปลงให้เป็นข้อความ
    String command = Serial.readStringUntil('\n');
    // ลบช่องว่างหน้าหลังออก
    command.trim();
    // ทำให้เป็นตัวพิมพ์เล็กทั้งหมดเพื่อเปรียบเทียบได้ง่าย
    command.toLowerCase();

    // เปรียบเทียบคำสั่งที่ได้รับ
    if (command == "เปิด") {
      // สั่งให้ LED ติด
      digitalWrite(ledPin, HIGH);
      ledOn = true;
      isBlinking = false;
      Serial.println("LED: ON");
    } else if (command == "ปิด") {
      // สั่งให้ LED ดับ
      digitalWrite(ledPin, LOW);
      ledOn = false;
      isBlinking = false;
      Serial.println("LED: OFF");
    } else if (command == "กระพริบ") {
      // ตั้งค่าให้เข้าสู่โหมดกระพริบ
      isBlinking = true;
      Serial.println("LED: BLINKING");
    } else {
      // ถ้าคำสั่งไม่ถูกต้อง
      Serial.println("Invalid command. Please use 'เปิด', 'ปิด', or 'กระพริบ'.");
    }
  }

  // หากอยู่ในโหมดกระพริบ ให้ทำงานตามเงื่อนไขนี้
  if (isBlinking) {
    // ใช้ฟังก์ชัน millis() เพื่อควบคุมเวลาโดยไม่บล็อกการทำงานของโค้ด
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;

      // สลับสถานะของ LED
      if (ledOn) {
        digitalWrite(ledPin, LOW);
      } else {
        digitalWrite(ledPin, HIGH);
      }
      ledOn = !ledOn; // กลับสถานะตัวแปร ledOn
    }
  }
}