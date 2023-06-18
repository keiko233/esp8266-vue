#include "databaseUtils.h"

const String databaseDirectoryName = "/database";

void initialDatabase() {
  if (!LittleFS.exists(databaseDirectoryName)) {
    LittleFS.mkdir(databaseDirectoryName);
  }
}

bool insertDatabase(const String& name, const String& data) {
  String filePath = databaseDirectoryName + "/" + name;
  
  File file = LittleFS.open(filePath, "w");
  if (!file) {
    Serial.println("Failed to create Database record file");
    return false;
  }
  
  file.print(data);
  
  file.close();
  return true;
}

String findDatabase(const String& name) {
  String filePath = databaseDirectoryName + "/" + name;
  
  if (!LittleFS.exists(filePath)) {
    return "";
  }
  
  File file = LittleFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open Database record file for reading");
    return "";
  }
  
  String data = file.readString();
  
  file.close();
  return data;
}

bool updateDatabase(const String& name, const String& newData) {
  String filePath = databaseDirectoryName + "/" + name;
  
  if (!LittleFS.exists(filePath)) {
    Serial.println("Database not found for updating");
    return false;
  }
  
  File file = LittleFS.open(filePath, "w");
  if (!file) {
    Serial.println("Failed to open Database record file for writing");
    return false;
  }
  
  file.print(newData);
  
  file.close();
  return true;
}

bool deleteDatabase(const String& name) {
  String filePath = databaseDirectoryName + "/" + name;
  
  if (!LittleFS.exists(filePath)) {
    Serial.println("Database not found for deleting");
    return false;
  }
  
  if (!LittleFS.remove(filePath)) {
    Serial.println("Failed to delete Database record file");
    return false;
  }
  
  return true;
}
