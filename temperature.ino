// --- 1. CONFIGURATION: NTC PHYSICAL CONSTANTS ---
// These values are specific to your NTC Thermistor. Check the datasheet!
const float NOMINAL_RESISTANCE = 10000;   // R0: Resistance at 25C (in ohms)
const float NOMINAL_TEMPERATURE = 25;     // T0: Temperature for NOMINAL_RESISTANCE (in Celsius)
const float B_VALUE = 3977;               // B-parameter (BETA value) of the thermistor
const float SERIES_RESISTOR = 10000;      // R_series: The value of the fixed resistor in the voltage divider (in ohms)

// --- 2. CONFIGURATION: HARDWARE CONSTANTS ---
const int ANALOG_PIN = 33;                // The analog pin on the ESP32 where the voltage divider output is connected
const float V_REF = 3.3;                  // Reference voltage (Vcc) for the sensor circuit (ESP32 is typically 3.3V)

// --- 3. CONFIGURATION: ADC CONSTANTS (ESP32 Specific) ---
const int ADC_RESOLUTION = 4095;          // ESP32's ADC is 12-bit (0 to 4095)

// --- 4. CALCULATION CONSTANTS ---
// Convert the nominal temperature from Celsius to Kelvin for the Steinhart-Hart B-parameter equation
const float KELVIN_OFFSET = 273.15;
const float NOMINAL_TEMP_K = NOMINAL_TEMPERATURE + KELVIN_OFFSET;


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("--- NTC Thermistor Reader Initialized ---");
  Serial.print("Reading from Analog Pin: ");
  Serial.println(ANALOG_PIN);
}

void loop() {
  // Read the analog value from the pin
  // For better stability, take an average of multiple readings
  float analog_value = 0;
  int num_readings = 10;
  for (int i = 0; i < num_readings; i++) {
    analog_value += analogRead(ANALOG_PIN);
    delay(1); // Small delay between readings
  }
  analog_value /= num_readings;
  
  // --- STEP 1: Calculate the voltage at the analog pin ---
  // The analog reading is converted back to a voltage
  float voltage = analog_value * (V_REF / ADC_RESOLUTION);

  // --- STEP 2: Calculate the NTC Resistance (RNTC) ---
  // R_NTC = R_series * ( (V_ref / V_out) - 1 )
  // This uses the voltage divider equation rearranged to solve for R_NTC
  float rntc = SERIES_RESISTOR * ( (V_REF / voltage) - 1 );
  
  // Guard against division by zero or negative resistance if sensor is unplugged
  if (rntc <= 0) {
    Serial.println("Error: Sensor disconnected or invalid reading.");
    delay(2000);
    return;
  }

  // --- STEP 3: Apply the Steinhart-Hart B-parameter Equation ---
  // 1/T = 1/T0 + (1/B) * ln(RNTC / R0)
  float steinhart;
  
  // First term: 1/T0
  steinhart = 1.0 / NOMINAL_TEMP_K;
  
  // Second term: (1/B) * ln(RNTC / R0)
  steinhart += (1.0 / B_VALUE) * log(rntc / NOMINAL_RESISTANCE);
  
  // The result is 1/T_kelvin, so we invert it to get T_kelvin
  float temperature_k = 1.0 / steinhart;

  // --- STEP 4: Convert Kelvin to Celsius ---
  float temperature_c = temperature_k - KELVIN_OFFSET;
  
  // --- Print Results ---
  Serial.print("Analog Reading: ");
  Serial.print(analog_value);
  Serial.print(" | RNTC: ");
  Serial.print(rntc);
  Serial.print(" ohms | Temp: ");
  Serial.print(temperature_c, 2); // Print with 2 decimal places
  Serial.println(" °C");
  
  delay(1000); // Wait one second before the next reading
}