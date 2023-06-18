#include "appUtils.h"

void softReboot(AsyncWebServerRequest* request) {
  ESP.restart();
}

void restart(AsyncWebServerRequest* request) {
  ESP.reset();
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}
