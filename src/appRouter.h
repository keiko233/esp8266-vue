#ifndef APP_ROUTER_H
#define APP_ROUTER_H

#define APP_VERSION "0.2.0"

#include <Arduino.h>
#include <ArduinoJson.h>
#include "main.h"

void getStatus(AsyncWebServerRequest* request);
void getFanSpeed(AsyncWebServerRequest* request);
void setFanSpeed(AsyncWebServerRequest* request);
void getWlanInfo(AsyncWebServerRequest* request);
void setWlanInfo(AsyncWebServerRequest* request);
void softReboot(AsyncWebServerRequest* request);
void restart(AsyncWebServerRequest* request);
void getLedInfo(AsyncWebServerRequest* request);
void setLedInfo(AsyncWebServerRequest* request);

#endif
