#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdarg>

class MockWiFiClass {
public:
    int scanNetworks() {
        return networks.size();
    }

    std::string SSID(int i) {
        return networks[i].ssid;
    }

    int RSSI(int i) {
        return networks[i].rssi;
    }

    void setMockNetworks(std::vector<std::pair<std::string, int>> mockData) {
        networks.clear();
        for (auto& [s, r] : mockData)
            networks.push_back({s, r});
    }

private:
    struct Network {
        std::string ssid;
        int rssi;
    };
    std::vector<Network> networks;
};

class MockSerialClass {
public:
    void begin(int baud) {
        std::cout << "Serial started at " << baud << " baud\n";
    }

    void println(const std::string& msg) {
        std::cout << msg << std::endl;
    }

    void printf(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
};

extern MockWiFiClass WiFi;
extern MockSerialClass Serial;
void delay(int ms);
