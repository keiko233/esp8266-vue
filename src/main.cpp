#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#define APP_VERSION "0.1.1"

const char* ssid = "Home";
const char* password = "12345687";

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

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void appLoadPinMode(void) {
  pinMode(FAN_PWM, OUTPUT);
  pinMode(FAN_TACH, INPUT);
}

void appLoadSerial(void) {
  Serial.begin(115200);
  Serial.println("Starting the LittleFS Webserver..");
}

void appLoadLittleFS(void) {
  // Begin LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void appLoadWlan(void) {
  // Connect to WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void appLoadRouter(void) {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.html", "text/html");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.js", "text/javascript");
  });

  server.on("/api/v1/status", HTTP_GET, getStatus);
}

void setup() {
  appLoadSerial();
  appLoadPinMode();
  appLoadLittleFS();
  appLoadWlan();
  appLoadRouter();
  server.onNotFound(notFound);
  server.begin();
}

void loop() {}