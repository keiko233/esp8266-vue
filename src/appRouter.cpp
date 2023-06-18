#include "appRouter.h"

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

void getFanSpeed(AsyncWebServerRequest* request) {
  StaticJsonDocument<128> jsonDoc;
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  rspObject["speed"] = findDatabase("fan_speed").toInt();

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
      rspObject["message"] = "The value of speed must between 0 and 255.";
    } else {
      analogWrite(FAN_PWM, param);

      if (updateDatabase("fan_speed", request->getParam("speed", true)->value())) {
        rspObject["status"] = "success";
        rspObject["message"] = "Speed set success.";
      } else {
        rspObject["status"] = "error";
        rspObject["message"] = "Database write failed.";
      }
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

void getWlanInfo(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(128);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  rspObject["ssid"] = findDatabase("wifi_ssid");
  rspObject["password"] = findDatabase("wifi_password");
  rspObject["mdns"] = findDatabase("wifi_mdns");

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void setWlanInfo(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(256);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  int httpStatus;
  String ssid;
  String password;
  String mdns;

  if (request->hasParam("ssid", true) && request->hasParam("password", true) && request->hasParam("mdns", true)) {
    httpStatus = 200;
    ssid = request->getParam("ssid", true)->value();
    password = request->getParam("password", true)->value();
    mdns = request->getParam("mdns", true)->value();

    if (ssid == "" || password == "" || mdns == "") {
      rspObject["status"] = "error";
      rspObject["message"] = "Parameter can'y be null or empty.";
    } else if (password.length() < 8) {
      rspObject["status"] = "error";
      rspObject["message"] = "Password must be greater than 8 characters.";
    } else {
      updateDatabase("wifi_ssid", ssid);
      updateDatabase("wifi_password", password);
      updateDatabase("wifi_mdns", mdns);

      rspObject["status"] = "success";
      rspObject["message"] = "Wlan info saved, please restart board to take effect.";
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

void getLedInfo(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(256);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  rspObject["ledRedBright"] = findDatabase("led_red_bright").toInt();
  rspObject["ledRedBlink"] = findDatabase("led_red_blink").toInt();
  rspObject["ledGreenBright"] = findDatabase("led_green_bright").toInt();
  rspObject["ledGreenBlink"] = findDatabase("led_green_blink").toInt();
  rspObject["ledBlueBright"] = findDatabase("led_blue_bright").toInt();
  rspObject["ledBlueBlink"] = findDatabase("led_blue_blink").toInt();

  String jsonResponse;
  serializeJson(rspObject, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void setLedInfo(AsyncWebServerRequest* request) {
  DynamicJsonDocument jsonDoc(256);
  JsonObject rspObject = jsonDoc.to<JsonObject>();

  int httpStatus;
  String led_red_bright;
  String led_red_blink;
  String led_green_bright;
  String led_green_blink;
  String led_blue_bright;
  String led_blue_blink;

  if (
    request->hasParam("ledRedBright", true) &&
    request->hasParam("ledRedBlink", true) &&
    request->hasParam("ledGreenBright", true) &&
    request->hasParam("ledGreenBlink", true) &&
    request->hasParam("ledBlueBright", true) &&
    request->hasParam("ledBlueBlink", true)
  ) {
    httpStatus = 200;
    led_red_bright  = request->getParam("ledRedBright", true)->value();
    led_red_blink  = request->getParam("ledRedBlink", true)->value();
    led_green_bright  = request->getParam("ledGreenBright", true)->value();
    led_green_blink  = request->getParam("ledGreenBlink", true)->value();
    led_blue_bright  = request->getParam("ledBlueBright", true)->value();
    led_blue_blink  = request->getParam("ledBlueBlink", true)->value();

    if (
      led_red_bright == "" ||
      led_red_blink == "" ||
      led_green_bright == "" ||
      led_green_blink == "" ||
      led_blue_bright == "" ||
      led_blue_blink == ""
    ) {
      rspObject["status"] = "error";
      rspObject["message"] = "Parameter can'y be null or empty.";
    } else {
      updateDatabase("led_red_bright", led_red_bright);
      updateDatabase("led_red_blink", led_red_blink);
      updateDatabase("led_green_bright", led_green_bright);
      updateDatabase("led_green_blink", led_green_blink);
      updateDatabase("led_blue_bright", led_blue_bright);
      updateDatabase("led_blue_blink", led_blue_blink);

      rspObject["status"] = "success";
      rspObject["message"] = "Led info saved.";
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
