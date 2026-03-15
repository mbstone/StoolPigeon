#ifdef UNIT_TEST
#include "ArduinoMock.h"
#else
#include <Arduino.h>
#include <WiFi.h>
#endif

#include "wifi_scan.h"

void run_wifi_scan() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Scanning Wi-Fi networks...");

    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("No networks found.");
    } else {
        Serial.printf("Found %d networks:\n", n);
        for (int i = 0; i < n; ++i) {
            Serial.printf("  %d: %s (RSSI: %d dBm)\n", i + 1,
                          WiFi.SSID(i).c_str(), WiFi.RSSI(i));
        }
    }
}
