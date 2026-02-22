#include <WiFi.h>
#include <HTTPClient.h>
#include <WifiCredentials.h>
#include <WebServerDetails.h>
#include <driver/rtc_io.h>
#include <ArduinoJson.h>

#define FSR_PIN A0           // Analog pin connected to the divider
#define R_FIXED 10000.0      // Fixed resistor value in ohms (10k)
#define FSR_THRESHOLD 1200           // Below this = no pressure
#define SLEEP_DELAY_US 10e6          // Sleep for 10 seconds (microseconds)
#define WAKEUP_GPIO  GPIO_NUM_2

// set variable for tracking last time high pressure observed 
unsigned long lastHighPressureTime = 0;
// set max allowed dead time without high pressure
const unsigned long PRESSURE_HOLD_MS = 30000;  // 30 seconds
// Build startup URL
String url = String("http://") + SERVER_HOST + ":" + SERVER_PORT + "/log";
// Initialize http client
HTTPClient http;
// Allocate the JSON document
JsonDocument doc;
// Initialize JsonBuffer
String jsonBuffer;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32 Wi-Fi HTTP Logging Test");

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());


  http.begin(url);
  Serial.println("Sending POST request to:");
  Serial.println(url);

  // fill the json document
  doc["fsr"] = 0;
  doc["voltage"] = 0.0;
  doc["resistance"] = 0.0;
  doc["message"] = "Stool Pigeon starting up";

  // 2. Serialize JSON to a string
  serializeJson(doc, jsonBuffer); // serializeJson() produces minified JSON output
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(jsonBuffer);
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

void loop() {
  delay(1000); // Wait before next reading
  int analogValue = analogRead(FSR_PIN);   // 12-bit ADC (0–4095)
  float voltage = (analogValue / 4095.0) * 3.3;  // Convert to volts

  // Calculate FSR resistance using voltage divider formula:
  // Vout = Vcc * R_fixed / (R_fixed + R_fsr)
  // Rearranged: R_fsr = (R_fixed * (Vcc - Vout)) / Vout
  float fsrResistance = (R_FIXED * (3.3 - voltage)) / voltage;

  if (analogValue >= FSR_THRESHOLD) {
    lastHighPressureTime = millis();  // Reset timer if pressure continues
  }

  // If pressure has been low for 5 seconds, go back to sleep for 10 seconds
  if (millis() - lastHighPressureTime > PRESSURE_HOLD_MS) {
    Serial.println("Pressure released. Sleeping in 3 seconds ..."); 
    // esp_sleep_enable_timer_wakeup(SLEEP_DELAY_US);
    esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1);
    esp_deep_sleep_start();
  }  


  // fill the json document
  doc["fsr"] = analogValue;
  doc["voltage"] = voltage;
  doc["resistance"] = fsrResistance;
  doc["message"] = "Mesurement";

  // post request time
  http.begin(url);
  Serial.println("Sending POST request to:");
  Serial.println(url);


  // 2. Serialize JSON to a string
  serializeJson(doc, jsonBuffer); // serializeJson() produces minified JSON output
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(jsonBuffer);
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
