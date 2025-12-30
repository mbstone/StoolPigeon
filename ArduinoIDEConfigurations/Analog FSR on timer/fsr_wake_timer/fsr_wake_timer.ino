// CONFIGURABLE PARAMETERS
#define FSR_PIN A0             // Analog pin connected to FSR voltage divider
#define FSR_THRESHOLD 1500     // Adjust based on seated pressure level (0–4095)
// values on thin film FSR:
// light touch: 1000-1200
// pinch: 2500
// hard pinch: 3500
#define SLEEP_DURATION_US 10e6 // Sleep for 10 seconds (10e6 microseconds)

void setup() {
  Serial.begin(115200);
  delay(200); // Let serial settle
  
  delay(5000); // <-- gives you time to upload a new sketch

  printWakeReason();

  int fsrValue = analogRead(FSR_PIN);
  Serial.print("FSR Reading: ");
  Serial.println(fsrValue);

  if (fsrValue < FSR_THRESHOLD) {
    Serial.println("No user detected. Going back to sleep.");
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION_US);
    esp_deep_sleep_start();
  } else {
    Serial.println("User detected! Staying awake...");
    // TODO: capture image, turn on LED, log time, send over Wi-Fi/BLE, etc.
  }
}

void loop() {
  // Optional: add timeout logic or go back to sleep after actions
}

void printWakeReason() {
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

  switch (cause) {
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Woke from timer.");
      break;
    case ESP_SLEEP_WAKEUP_UNDEFINED:
      Serial.println("Wake not from deep sleep.");
      break;
    default:
      Serial.printf("Wakeup cause: %d\n", cause);
      break;
  }
}
