#include <WiFi.h>
#include <HTTPClient.h>
#include <WifiCredentials.h>
#include <WebServerDetails.h>

#define FSR_PIN A0           // Analog pin connected to the divider
#define R_FIXED 10000.0      // Fixed resistor value in ohms (10k)
#define FSR_THRESHOLD 1200           // Below this = no pressure
#define SLEEP_DELAY_US 10e6          // Sleep for 10 seconds (microseconds)

unsigned long lastHighPressureTime = 0;
const unsigned long PRESSURE_HOLD_MS = 5000;  // 5 seconds

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

  // Build startup URL
  String url = String("http://") + SERVER_HOST + ":" + SERVER_PORT + "/log";
  String jsonPayload = String("{\"event\":\"startup\",\"ip\":\"") + WiFi.localIP().toString() + "\"}";
  // Send HTTP GET request
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

  // If pressure has been low for 5 seconds, go back to sleep
  if (millis() - lastHighPressureTime > PRESSURE_HOLD_MS) {
    Serial.println("Pressure released. Sleeping in 3 seconds ..."); 
    esp_sleep_enable_timer_wakeup(SLEEP_DELAY_US);
    esp_deep_sleep_start();
  }  

  String url = String("http://") + SERVER_HOST + ":" + SERVER_PORT +
    "/log?fsr=" + String(analogValue, 4) +
    "&voltage=" + String(voltage, 4) +
    "&resistance=" + String(fsrResistance, 4);  

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
