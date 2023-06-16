#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "appLoad.h"
#include "databaseUtils.h"

#define APP_VERSION "0.1.2"

// D5
const int FAN_PWM = 14;
// D6
const int FAN_TACH = 12;

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
      rspObject["message"] = "The value of speed is too large.";
    } else {
      analogWrite(FAN_PWM, param);
      rspObject["status"] = "success";
      rspObject["message"] = "Speed set success.";
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
  DynamicJsonDocument jsonDoc(128);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  rspObject["speed"] = getDBObject()["control"]["fan"]["speed"];

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void appLoadPinMode(void) {
  pinMode(FAN_PWM, OUTPUT);
  pinMode(FAN_TACH, INPUT);
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
}

void setup() {
  appLoadSerial();
  appLoadPinMode();
  appLoadLittleFS();

  const char* ssid = getDBObject()["wifi"]["ssid"];
  const char* password = getDBObject()["wifi"]["password"];
  const char* mdns = getDBObject()["wifi"]["mdns"];

  appLoadWlan(ssid, password);
  appLoadMDns(mdns);
  appLoadRouter();
  server.onNotFound(notFound);
  server.begin();
}

void loop() {}