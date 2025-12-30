#define FSR_PIN A0           // Analog pin connected to the divider
#define R_FIXED 10000.0      // Fixed resistor value in ohms (10k)

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial connection
  delay(500);
  Serial.println("Reading FSR resistance...");
}

void loop() {
  int analogValue = analogRead(FSR_PIN);   // 12-bit ADC (0–4095)
  float voltage = (analogValue / 4095.0) * 3.3;  // Convert to volts

  // Calculate FSR resistance using voltage divider formula:
  // Vout = Vcc * R_fixed / (R_fixed + R_fsr)
  // Rearranged: R_fsr = (R_fixed * (Vcc - Vout)) / Vout
  float fsrResistance = (R_FIXED * (3.3 - voltage)) / voltage;

  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | FSR Resistance: ");
  Serial.print(fsrResistance, 1);
  Serial.println(" ohms");

  delay(500); // Sample every half second
}
