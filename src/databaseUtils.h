
#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include <ArduinoJson.h>
#include <LittleFS.h>

JsonObject getDBObject(DynamicJsonDocument& doc);
bool setDBObject(const JsonObject& data);

#endif
