#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char* ssid = "Home";
const char* password = "12345687";

// Webserver
AsyncWebServer server(80);

String PARAM_MESSAGE = "status";

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting the LittleFS Webserver..");

  // Begin LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Connect to WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.html", "text/html");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.js", "text/javascript");
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, GET: " + message);
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest* request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop() {}