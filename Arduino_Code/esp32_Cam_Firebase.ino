#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"

// ══════════════════════════════════════════
//   YOUR SETTINGS — EDIT THESE 3 LINES
// ══════════════════════════════════════════
#define WIFI_SSID      "YOUR_WIFI_SSID"
#define WIFI_PASSWORD  "YOUR_WIFI_PASSWORD"
#define DATABASE_URL   "https://your-project-default-rtdb.firebaseio.com/"

#define IR_PIN    13   // GPIO pin connected to IR sensor OUT
#define FLASH_PIN  4   // ESP32-CAM built-in flash LED

// ══════════════════════════════════════════
//   ESP32-CAM PINS (AI-Thinker — DO NOT CHANGE)
// ══════════════════════════════════════════
#define PWDN_GPIO   32
#define RESET_GPIO  -1
#define XCLK_GPIO    0
#define SIOD_GPIO   26
#define SIOC_GPIO   27
#define Y9_GPIO     35
#define Y8_GPIO     34
#define Y7_GPIO     39
#define Y6_GPIO     36
#define Y5_GPIO     21
#define Y4_GPIO     19
#define Y3_GPIO     18
#define Y2_GPIO      5
#define VSYNC_GPIO  25
#define HREF_GPIO   23
#define PCLK_GPIO   22

// ══════════════════════════════════════════
//   BASE64 ENCODER
// ══════════════════════════════════════════
static const char b64chars[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

String base64Encode(uint8_t* data, size_t len) {
  String result = "";
  result.reserve((len / 3 + 1) * 4 + 4);
  int i = 0;
  uint8_t b3[3], b4[4];
  size_t remaining = len;
  while (remaining--) {
    b3[i++] = *data++;
    if (i == 3) {
      b4[0] = (b3[0] & 0xfc) >> 2;
      b4[1] = ((b3[0] & 0x03) << 4) + ((b3[1] & 0xf0) >> 4);
      b4[2] = ((b3[1] & 0x0f) << 2) + ((b3[2] & 0xc0) >> 6);
      b4[3] =  b3[2] & 0x3f;
      for (int j = 0; j < 4; j++) result += b64chars[b4[j]];
      i = 0;
    }
  }
  if (i) {
    for (int j = i; j < 3; j++) b3[j] = 0;
    b4[0] = (b3[0] & 0xfc) >> 2;
    b4[1] = ((b3[0] & 0x03) << 4) + ((b3[1] & 0xf0) >> 4);
    b4[2] = ((b3[1] & 0x0f) << 2) + ((b3[2] & 0xc0) >> 6);
    for (int j = 0; j < i + 1; j++) result += b64chars[b4[j]];
    while (i++ < 3) result += '=';
  }
  return result;
}

// ══════════════════════════════════════════
//   CAMERA INIT
// ══════════════════════════════════════════
void initCamera() {
  camera_config_t cfg;
  cfg.ledc_channel = LEDC_CHANNEL_0;
  cfg.ledc_timer   = LEDC_TIMER_0;
  cfg.pin_d0  = Y2_GPIO;  cfg.pin_d1  = Y3_GPIO;
  cfg.pin_d2  = Y4_GPIO;  cfg.pin_d3  = Y5_GPIO;
  cfg.pin_d4  = Y6_GPIO;  cfg.pin_d5  = Y7_GPIO;
  cfg.pin_d6  = Y8_GPIO;  cfg.pin_d7  = Y9_GPIO;
  cfg.pin_xclk     = XCLK_GPIO;
  cfg.pin_pclk     = PCLK_GPIO;
  cfg.pin_vsync    = VSYNC_GPIO;
  cfg.pin_href     = HREF_GPIO;
  cfg.pin_sscb_sda = SIOD_GPIO;
  cfg.pin_sscb_scl = SIOC_GPIO;
  cfg.pin_pwdn     = PWDN_GPIO;
  cfg.pin_reset    = RESET_GPIO;
  cfg.xclk_freq_hz = 20000000;
  cfg.pixel_format = PIXFORMAT_JPEG;
  cfg.frame_size   = FRAMESIZE_QVGA;  // 320x240
  cfg.jpeg_quality = 20;
  cfg.fb_count     = 1;

  if (esp_camera_init(&cfg) != ESP_OK) {
    Serial.println("Camera init failed! Restarting...");
    delay(2000);
    ESP.restart();
  }
  Serial.println("Camera ready");
}

// ══════════════════════════════════════════
//   SEND TO FIREBASE USING REST API
// ══════════════════════════════════════════
bool sendToFirebase(String b64image) {
  WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(15);

  HTTPClient http;

  String url = String(DATABASE_URL) + "/current_photo.json";

  Serial.println("Connecting to Firebase...");

  if (!http.begin(client, url)) {
    Serial.println("HTTP begin failed");
    return false;
  }

  http.addHeader("Content-Type", "application/json");
  http.setTimeout(15000);

  String payload = "{";
  payload += "\"photo\":\"" + b64image + "\",";
  payload += "\"motion\":true,";
  payload += "\"timestamp\":" + String(millis());
  payload += "}";

  Serial.println("Sending data...");
  int httpCode = http.PUT(payload);

  if (httpCode > 0) {
    Serial.printf("HTTP Response: %d\n", httpCode);
    if (httpCode == 200 || httpCode == 204) {
      Serial.println("SUCCESS! Data sent to Firebase!");
      http.end();
      return true;
    } else {
      String response = http.getString();
      Serial.println("Error response: " + response);
    }
  } else {
    Serial.printf("HTTP failed: %s\n",
      http.errorToString(httpCode).c_str());
  }

  http.end();
  return false;
}

// ══════════════════════════════════════════
//   CAPTURE AND SEND
// ══════════════════════════════════════════
void captureAndSend() {

  // ── Blink twice = motion detected ──
  digitalWrite(FLASH_PIN, HIGH); delay(100);
  digitalWrite(FLASH_PIN, LOW);  delay(100);
  digitalWrite(FLASH_PIN, HIGH); delay(100);
  digitalWrite(FLASH_PIN, LOW);  delay(100);

  Serial.println("Capturing image...");

  // Flash ON for better photo quality
  digitalWrite(FLASH_PIN, HIGH);

  // Discard first frame (camera warm up)
  camera_fb_t* fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  delay(200);

  // Take actual photo with flash ON
  fb = esp_camera_fb_get();

  // Flash OFF immediately after capture
  digitalWrite(FLASH_PIN, LOW);

  if (!fb) {
    Serial.println("Capture failed!");
    // Long blink = capture error
    digitalWrite(FLASH_PIN, HIGH); delay(1000);
    digitalWrite(FLASH_PIN, LOW);
    return;
  }

  Serial.printf("Image size: %d bytes\n", fb->len);

  // Encode to Base64
  String b64 = base64Encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  Serial.printf("Base64 length: %d chars\n", b64.length());

  // Send to Firebase
  if (sendToFirebase(b64)) {
    // ── Blink 3 times = upload success ──
    for (int i = 0; i < 3; i++) {
      digitalWrite(FLASH_PIN, HIGH); delay(100);
      digitalWrite(FLASH_PIN, LOW);  delay(100);
    }
    Serial.println("════ Photo uploaded successfully! ════");
  } else {
    // ── Long blink = upload failed ──
    digitalWrite(FLASH_PIN, HIGH); delay(1000);
    digitalWrite(FLASH_PIN, LOW);
    Serial.println("Upload failed — will retry on next motion");
  }
}

// ══════════════════════════════════════════
//   SETUP
// ══════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  Serial.println("\nESP32-CAM Motion Detector Starting...");

  // Flash pin setup
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW);  // OFF at start

  // IR sensor pin setup
  pinMode(IR_PIN, INPUT);

  // Startup blink — 1 slow blink = system starting
  digitalWrite(FLASH_PIN, HIGH); delay(500);
  digitalWrite(FLASH_PIN, LOW);  delay(500);

  // Init camera
  initCamera();

  // Connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting WiFi");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry++;
    if (retry > 40) {
      Serial.println("\nWiFi timeout! Restarting...");
      // Rapid blink = WiFi failed
      for (int i = 0; i < 10; i++) {
        digitalWrite(FLASH_PIN, HIGH); delay(100);
        digitalWrite(FLASH_PIN, LOW);  delay(100);
      }
      delay(1000);
      ESP.restart();
    }
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.println("IP: " + WiFi.localIP().toString());

  // WiFi connected — 2 quick blinks
  digitalWrite(FLASH_PIN, HIGH); delay(200);
  digitalWrite(FLASH_PIN, LOW);  delay(200);
  digitalWrite(FLASH_PIN, HIGH); delay(200);
  digitalWrite(FLASH_PIN, LOW);

  Serial.println("════════════════════════════════");
  Serial.println("Ready — waiting for motion...");
  Serial.println("════════════════════════════════");
}

// ══════════════════════════════════════════
//   LOOP
// ══════════════════════════════════════════
void loop() {
  // Auto reconnect WiFi if dropped
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost! Reconnecting...");
    WiFi.reconnect();
    delay(5000);
    return;
  }

  // Detect motion
  if (digitalRead(IR_PIN) == HIGH) {
    Serial.println("\nMotion detected!");
    captureAndSend();
    Serial.println("Cooldown 8 seconds...");
    delay(8000);
  }
}