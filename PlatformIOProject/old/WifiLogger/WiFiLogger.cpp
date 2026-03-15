// WifiLogger.cpp
#include "WifiLogger.h"

WifiLogger::WifiLogger(const char* serverHost, int serverPort) {
  _serverHost = String(serverHost);
  _serverPort = serverPort;
}

void WifiLogger::begin(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void WifiLogger::logMessage(const String& message) {
  String url = "http://" + _serverHost + ":" + _serverPort + "/log?message=" + message;
  HTTPClient http;
  http.begin(url);
  Serial.println("Sending GET request to:");
  Serial.println(url);

  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("HTTP Response Code: %d\n", httpCode);
    String payload = http.getString();
    Serial.println("Response:");
    Serial.println(payload);
  } else {
    Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void WifiLogger::logSensorData(float fsr, float voltage, float resistance) {
  String url = "http://" + _serverHost + ":" + _serverPort +
               "/log?fsr=" + String(fsr, 4) +
               "&voltage=" + String(voltage, 4) +
               "&resistance=" + String(resistance, 4);
  HTTPClient http;
  http.begin(url);
  Serial.println("Sending GET request to:");
  Serial.println(url);

  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("HTTP Response Code: %d\n", httpCode);
    String payload = http.getString();
    Serial.println("Response:");
    Serial.println(payload);
  } else {
    Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
