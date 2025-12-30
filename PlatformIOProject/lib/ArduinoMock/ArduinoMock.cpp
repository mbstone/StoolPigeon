#include "ArduinoMock.h"
#include <thread>
#include <chrono>

MockWiFiClass WiFi;
MockSerialClass Serial;

void delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
