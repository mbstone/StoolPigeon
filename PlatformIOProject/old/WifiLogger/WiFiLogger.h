// WifiLogger.h
#ifndef WIFI_LOGGER_H
#define WIFI_LOGGER_H

#include <WiFi.h>
#include <HTTPClient.h>

class WifiLogger {
  public:
    WifiLogger(const char* serverHost, int serverPort);
    void begin(const char* ssid, const char* password);
    void logMessage(const String& message);
    void logSensorData(float fsr, float voltage, float resistance);

  private:
    String _serverHost;
    int _serverPort;
};

#endif // WIFI_LOGGER_H
