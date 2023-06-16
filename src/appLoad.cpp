#include "appLoad.h" // 包含头文件

void appLoadLittleFS(void) {
  // Begin LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void appLoadSerial(void) {
  Serial.begin(115200);
  Serial.println("Starting the LittleFS Webserver..");
}

void appLoadWlan(String ssid, String password) {
  // Connect to WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
