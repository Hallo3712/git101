#include "OV2640.h"
#include <WiFi.h>
#include <WiFiClient.h>

#include "SimStreamer.h"
#include "OV2640Streamer.h"
#include "CRtspSession.h"

#define ENABLE_OLED // ถ้ามีจอ OLED ให้ใช้ได้
#define ENABLE_RTSPSERVER

#ifdef ENABLE_OLED
#include "SSD1306.h"
#define OLED_ADDRESS 0x3c
#define I2C_SDA 14
#define I2C_SCL 13
SSD1306Wire display(OLED_ADDRESS, I2C_SDA, I2C_SCL, GEOMETRY_128_32);
bool hasDisplay;
#endif

// ใส่ชื่อ WiFi และรหัสผ่านของคุณ
const char* ssid = "Test";
const char* password = "Test987654321";

OV2640 cam;
WiFiServer rtspServer(8554);

#ifdef ENABLE_OLED
#define LCD_MESSAGE(msg) lcdMessage(msg)
#else
#define LCD_MESSAGE(msg)
#endif

#ifdef ENABLE_OLED
void lcdMessage(String msg)
{
    if (hasDisplay) {
        display.clear();
        display.drawString(64, 16, msg);
        display.display();
    }
}
#endif

CStreamer* streamer;

void setup()
{
#ifdef ENABLE_OLED
    hasDisplay = display.init();
    if (hasDisplay) {
        display.flipScreenVertically();
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
    }
#endif

    Serial.begin(115200);
    cam.init(esp32cam_config);

    // ตั้งความละเอียดและคุณภาพภาพ
    cam.setResolution(FRAMESIZE_QVGA);   // 320x240
    cam.setQuality(12); // ค่า JPEG (ต่ำ = ชัด)

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    LCD_MESSAGE("Connecting WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    IPAddress ip = WiFi.localIP();
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println(ip);
    LCD_MESSAGE(ip.toString());

    // เริ่ม RTSP server
    rtspServer.begin();
    streamer = new OV2640Streamer(cam); // ใช้กล้องจริง
}

void loop()
{
    static uint32_t lastimage = millis();
    uint32_t msecPerFrame = 66; // ประมาณ 15 FPS
    uint32_t now = millis();

    streamer->handleRequests(0);

    if (streamer->anySessions()) {
        if (now > lastimage + msecPerFrame || now < lastimage) {
            streamer->streamImage(now);
            lastimage = now;
        }
    }

    WiFiClient rtspClient = rtspServer.accept();
    if (rtspClient) {
        Serial.print("RTSP client connected: ");
        Serial.println(rtspClient.remoteIP());
        streamer->addSession(rtspClient);
    }
}
