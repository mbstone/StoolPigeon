#include <WiFi.h>
#include <HTTPClient.h>
#include <WifiCredentials.h>
#include <WebServerDetails.h>


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

  // Build test URL
  String url = String("http://") + SERVER_HOST + ":" + SERVER_PORT +
               "/log?fsr=1234&voltage=2.5&resistance=8900";

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

  // Optional: go to sleep or halt
  Serial.println("Done. Halting...");
  while (true);  // Stop here
}

void loop() {
  // Nothing here
}
