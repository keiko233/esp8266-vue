#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "appLoad.h"
#include "databaseUtils.h"
#include "envSetup.h"

#define APP_VERSION "0.1.2"

// D5
const int FAN_PWM = 14;
// D6
const int FAN_TACH = 12;
// TX
const int LED_R = 1;
// D4
const int LED_G = 2;
// D0
const int LED_B = 16;

// Webserver
AsyncWebServer server(80);

void getStatus(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(1024);
  JsonObject rspObject = jsonDoc.to<JsonObject>();
  JsonObject sysObject = rspObject.createNestedObject("system");
  JsonObject archObject = sysObject.createNestedObject("arch");
  JsonObject memObject = rspObject.createNestedObject("memory");
  JsonObject fsObject = rspObject.createNestedObject("file_system");
  JsonObject apObject = rspObject.createNestedObject("access_point");
  JsonObject staObject = rspObject.createNestedObject("static_access_point");

  sysObject["model"] = "NodeMCU v1.0";
  sysObject["firmware"] = APP_VERSION;
  sysObject["esp_sdk"] = ESP.getSdkVersion();
  sysObject["cpu_freq"] = ESP.getCpuFreqMHz();

  archObject["manufacturer"] = "Espressif";
  archObject["model"] = ESP.getChipId();

  memObject["free"] = ESP.getFreeHeap();

  fsObject["total"] = ESP.getFlashChipSize();

  apObject["ssid"] = WiFi.softAPSSID();
  apObject["num"] = WiFi.softAPgetStationNum();

  staObject["ssid"] = WiFi.SSID();
  staObject["status"] =
      WiFi.status() == WL_CONNECTED ? "connected" : "disconnected";

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void setFanSpeed(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(256);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  int httpStatus;
  int param;

  if (request->hasParam("speed", true)) {
    httpStatus = 200;
    param = request->getParam("speed", true)->value().toInt();
    if (param > 255 || param < 0) {
      rspObject["status"] = "error";
      rspObject["message"] = "The value of speed must between 0 and 255.";
    } else {
      analogWrite(FAN_PWM, param);

      if (updateDatabase("fan_speed", request->getParam("speed", true)->value())) {
        rspObject["status"] = "success";
        rspObject["message"] = "Speed set success.";
      } else {
        rspObject["status"] = "error";
        rspObject["message"] = "Database write failed.";
      }
    }
  } else {
    httpStatus = 400;
    rspObject["status"] = "error";
    rspObject["message"] = "No speed parameter provided.";
  }

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(httpStatus, "application/json", jsonResponse);
}

void getFanSpeed(AsyncWebServerRequest* request) {
  StaticJsonDocument<128> jsonDoc;
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  String fan_speed = findDatabase("fan_speed");
  int speed = fan_speed.toInt();
  rspObject["speed"] = speed;

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void getWlanInfo(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(128);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  String wifi_ssid = findDatabase("wifi_ssid");
  String wifi_password = findDatabase("wifi_password");
  String wifi_mdns = findDatabase("wifi_mdns");

  rspObject["ssid"] = wifi_ssid;
  rspObject["password"] = wifi_password;
  rspObject["mdns"] = wifi_mdns;

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void setWlanInfo(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(256);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  int httpStatus;
  String ssid;
  String password;
  String mdns;

  if (request->hasParam("ssid", true) && request->hasParam("password", true) && request->hasParam("mdns", true)) {
    httpStatus = 200;
    ssid = request->getParam("ssid", true)->value();
    password = request->getParam("password", true)->value();
    mdns = request->getParam("mdns", true)->value();

    if (ssid == "" || password == "" || mdns == "") {
      rspObject["status"] = "error";
      rspObject["message"] = "Parameter can'y be null or empty.";
    } else if (password.length() < 8) {
      rspObject["status"] = "error";
      rspObject["message"] = "Password must be greater than 8 characters.";
    } else {
      updateDatabase("wifi_ssid", ssid);
      updateDatabase("wifi_password", password);
      updateDatabase("wifi_mdns", mdns);

      rspObject["status"] = "success";
      rspObject["message"] = "Wlan info saved, please restart board to take effect.";
    }
  } else {
    httpStatus = 400;
    rspObject["status"] = "error";
    rspObject["message"] = "No speed parameter provided.";
  }

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(httpStatus, "application/json", jsonResponse);
}

void softReboot(AsyncWebServerRequest* request) {
  ESP.restart();
}

void restart(AsyncWebServerRequest* request) {
  ESP.reset();
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

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
}

void appLoadSetup(void) {
  String fan_speed = findDatabase("fan_speed");
  int speed = fan_speed.toInt();
  analogWrite(FAN_PWM, speed);
}

void setup() {
  appLoadSerial();
  appLoadPinMode();
  appLoadLittleFS();
  appLoadLittleFSSetup();
  appLoadSetup();

  String wifi_ssid = findDatabase("wifi_ssid");
  String wifi_password = findDatabase("wifi_password");
  String wifi_mdns = findDatabase("wifi_mdns");

  appLoadWlan(wifi_ssid, wifi_password);
  appLoadMDns(wifi_mdns);
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
  int led_red_bright = findDatabase("led_red_bright").toInt();
  int led_red_blink = findDatabase("led_red_blink").toInt();
  int led_green_bright = findDatabase("led_green_bright").toInt();
  int led_green_blink = findDatabase("led_green_blink").toInt();
  int led_blue_bright = findDatabase("led_blue_bright").toInt();
  int led_blue_blink = findDatabase("led_blue_blink").toInt();

  controlLed(LED_R, led_red_bright);
  controlLed(LED_R, led_red_blink, true);
  controlLed(LED_G, led_green_bright);
  controlLed(LED_G, led_green_blink, true);
  controlLed(LED_B, led_blue_bright);
  controlLed(LED_B, led_blue_blink, true);
}

void loop() {
  ledLoop();
}