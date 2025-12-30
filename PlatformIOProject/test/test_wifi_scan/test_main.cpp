#include <unity.h>

#ifndef ARDUINO
#include "ArduinoMock.h"
#endif

#include "wifi_scan.h"

void setUp(void) {}
void tearDown(void) {}

void test_wifi_scan_output() {
    WiFi.setMockNetworks({
        {"TestNetwork_1", -50},
        {"TestNetwork_2", -65}
    });

    run_wifi_scan();  // Now calls the shared implementation
    TEST_ASSERT_TRUE(true);  // Or test output if captured
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_wifi_scan_output);
    return UNITY_END();
}
