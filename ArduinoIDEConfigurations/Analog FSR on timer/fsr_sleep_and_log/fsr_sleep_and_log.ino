#include <WiFi.h>
#include <HTTPClient.h>
#include <WifiCredentials.h>
#include <WebServerDetails.h>

#define FSR_PIN A0
#define R_FIXED 10000.0               // 10kΩ resistor in voltage divider
#define FSR_THRESHOLD 1200           // Below this = no pressure
#define SLEEP_DELAY_US 10e6          // Sleep for 10 seconds (microseconds)

unsigned long lastHighPressureTime = 0;
const unsigned long PRESSURE_HOLD_MS = 5000;  // 5 seconds

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(500);

  Serial.println("ESP32 Wake with FSR and Wi-Fi HTTP Logging Test");

  printWakeReason();

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

  // Read initial FSR value
  int fsr = analogRead(FSR_PIN);
  float voltage = (fsr / 4095.0) * 3.3;
  float resistance = (R_FIXED * (3.3 - voltage)) / voltage;
  Serial.print("Initial FSR Reading: ");
  Serial.println(fsr);  

  char queryParams[30]
  sprintf(queryParams, "fsr=%d&voltage=%1.1&resistance=%4.0", voltage)

  // log values to webserver
  String url = String("http://") + SERVER_HOST + ":" + SERVER_PORT +
               String("/log?") + queryParams;

  if (fsr < FSR_THRESHOLD) {
    Serial.println("No pressure detected. Going back to sleep...");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // Turn on before sleep
    delay(3000);  // helps to see light
    esp_sleep_enable_timer_wakeup(SLEEP_DELAY_US);
    esp_deep_sleep_start();
  } else {
    Serial.println("Pressure detected! Staying awake...");
    lastHighPressureTime = millis();  // Start tracking
  }
}

void loop() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Turn on before sleep  
  int fsr = analogRead(FSR_PIN);
  float voltage = (fsr / 4095.0) * 3.3;
  float resistance = (R_FIXED * (3.3 - voltage)) / voltage;

  Serial.print("Analog: ");
  Serial.print(fsr);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | FSR Resistance: ");
  Serial.print(resistance, 1);
  Serial.println(" ohms");

  if (fsr >= FSR_THRESHOLD) {
    lastHighPressureTime = millis();  // Reset timer if pressure continues
  }

  // If pressure has been low for 5 seconds, go back to sleep
  if (millis() - lastHighPressureTime > PRESSURE_HOLD_MS) {
    Serial.println("Pressure released. Sleeping in 3 seconds ...");
    delay(3000);  // Let message flush    
    esp_sleep_enable_timer_wakeup(SLEEP_DELAY_US);
    esp_deep_sleep_start();
  }

  delay(500);  // Sample interval
}

void printWakeReason() {
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  switch (cause) {
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup from timer.");
      break;
    case ESP_SLEEP_WAKEUP_UNDEFINED:
      Serial.println("Normal power-on boot.");
      break;
    default:
      Serial.printf("Wakeup cause: %d\n", cause);
      break;
  }
}
