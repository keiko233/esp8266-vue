#include "databaseUtils.h"

const char* dbpath = "/db.json";

JsonObject getDBObject(DynamicJsonDocument& doc) {
  File file = LittleFS.open(dbpath, "r");
  if (!file) {
    Serial.println("Database file read failed.");
    return JsonObject();
  }

  size_t fileSize = file.size();
  // 创建缓冲区来保存文件内容
  std::unique_ptr<char[]> buf(new char[fileSize]);
  file.readBytes(buf.get(), fileSize);
  file.close();
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Database Deserialization Error.");
    return JsonObject();
  }

  return doc.as<JsonObject>();
}

bool setDBObject(const JsonObject& data) {
  File file = LittleFS.open(dbpath, "w");
  if (!file) {
    Serial.println("Database file read failed.");
    return false;
  }

  char buffer[256];
  size_t bufferSize = sizeof(buffer);
  size_t bytesWritten = serializeJson(data, buffer, bufferSize);
  size_t bytesSaved = file.write(reinterpret_cast<const uint8_t*>(buffer), bytesWritten);

  file.close();
  
  return bytesWritten == bytesSaved;
}
