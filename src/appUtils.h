#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

void softReboot(AsyncWebServerRequest* request);
void restart(AsyncWebServerRequest* request);
void notFound(AsyncWebServerRequest* request);

#endif
