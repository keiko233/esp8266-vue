
#ifndef APP_LOAD_H
#define APP_LOAD_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

void appLoadLittleFS(void);
void appLoadSerial(void);
void appLoadWlan(String ssid, String password);
void appLoadMDns(String name);

#endif
