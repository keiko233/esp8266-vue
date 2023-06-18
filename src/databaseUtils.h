
#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include <LittleFS.h>

void initialDatabase();
bool insertDatabase(const String& name, const String& data);
String findDatabase(const String& name);
bool updateDatabase(const String& name, const String& newData);
bool deleteDatabase(const String& name);

#endif
