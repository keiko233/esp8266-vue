#include "main.h"

// Webserver
AsyncWebServer server(80);

void appLoadPinMode(void) {
  pinMode(FAN_PWM, OUTPUT);
  pinMode(FAN_TACH, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void appLoadRouter(void) {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.html", "text/html");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.js", "text/javascript");
  });

  server.on("/api/status", HTTP_GET, getStatus);
  server.on("/api/fan", HTTP_GET, getFanSpeed);
  server.on("/api/fan", HTTP_POST, setFanSpeed);
  server.on("/api/settings/wlan", HTTP_GET, getWlanInfo);
  server.on("/api/settings/wlan", HTTP_POST, setWlanInfo);
  server.on("/api/softreboot", HTTP_GET, softReboot);
  server.on("/api/restart", HTTP_GET, restart);
  server.on("/api/led", HTTP_GET, getLedInfo);
  server.on("/api/led", HTTP_POST, setLedInfo);
}

void setup() {
  appLoadSerial();
  appLoadPinMode();
  appLoadLittleFS();
  appLoadLittleFSSetup();
  appLoadSetup();
  appLoadWlan(findDatabase("wifi_ssid"), findDatabase("wifi_password"));
  appLoadMDns(findDatabase("wifi_mdns"));
  appLoadRouter();
  server.onNotFound(notFound);
  server.begin();
}

void controlLed(int ledPin, int duration, bool shouldBlink = false, int blinkInterval = 500) {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_G, LOW);

  if (shouldBlink) {
    int blinkCount = duration / (2 * blinkInterval);
    for (int i = 0; i < blinkCount; i++) {
      digitalWrite(ledPin, LOW);
      delay(blinkInterval);
      digitalWrite(ledPin, HIGH);
      delay(blinkInterval);
    }
  } else {
    digitalWrite(ledPin, HIGH);
    delay(duration);
  }
}

void ledLoop() {
  controlLed(LED_R, findDatabase("led_red_bright").toInt());
  controlLed(LED_R, findDatabase("led_red_blink").toInt(), true);
  controlLed(LED_G, findDatabase("led_green_bright").toInt());
  controlLed(LED_G, findDatabase("led_green_blink").toInt(), true);
  controlLed(LED_B, findDatabase("led_blue_bright").toInt());
  controlLed(LED_B, findDatabase("led_blue_blink").toInt(), true);
}

void loop() {
  ledLoop();
}