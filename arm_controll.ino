#include "BluetoothSerial.h"
#include "ESP32Servo.h"

// --- Configuration ---

// Define the name the ESP32 will show up as on Bluetooth
#define BT_DEVICE_NAME "SERVO_PLEASE_WORK" 

// Define the GPIO pins for each of the 5 servos
// **NOTE: PINS 34, 39, 35 are INPUT ONLY. I recommend using safe output pins instead.**
const int PIN_SERVO1 = 32;
const int PIN_SERVO2 = 33; // CHANGED (Recommended safe output pin)
const int PIN_SERVO3 = 27; // CHANGED (Recommended safe output pin)
const int PIN_SERVO4 = 13;
const int PIN_SERVO5 = 35; // CHANGED (Recommended safe output pin)

// --- Global Objects ---

// Create the Bluetooth Serial object
BluetoothSerial SerialBT;

// Create 5 individual Servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Variable to store the incoming Bluetooth command string
String commandString = "";

// --- Function Declaration ---
void parseAndControlIndividualServo(String command);

// ==========================================================
// SETUP FUNCTION
// ==========================================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- ESP32 Individual Servo Controller Ready ---");
  
  // Initialize Bluetooth Serial
  SerialBT.begin(BT_DEVICE_NAME);
  Serial.printf("Bluetooth Device Name: %s\n", BT_DEVICE_NAME);
  Serial.println("Waiting for a Bluetooth connection...");

  // Attach and Initialize Each Servo Individually
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);
  servo4.attach(PIN_SERVO4);
  servo5.attach(PIN_SERVO5);

  // Set all servos to an initial angle (e.g., 90 degrees)
  servo1.write(90); 
  servo2.write(90); 
  servo3.write(90); 
  servo4.write(90); 
  servo5.write(90); 
  
  Serial.println("All 5 servos initialized to 90 degrees.");
}

// ==========================================================
// LOOP FUNCTION (WITH DEBUG PRINT)
// ==========================================================
void loop() {
  // Check if any data is available from Bluetooth
  while (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    
    // Check for the newline character ('\n') which signals the end of a command string
    if (incomingChar == '\n') {
      
      // *** DEBUGGING LINE: Prints the full string received from the app ***
      Serial.print("RAW CMD RECEIVED: [");
      Serial.print(commandString);
      Serial.println("]");
      // *******************************************************************
      
      // A full command has been received, process it
      parseAndControlIndividualServo(commandString);
      
      // Clear the command string for the next command
      commandString = ""; 
    } else {
      // Append the received character to the current command string
      commandString += incomingChar;
    }
  }

  delay(5);
}

// ==========================================================
// PARSING AND CONTROL FUNCTION (No Change)
// ==========================================================
/**
 * @brief Parses the command string and controls the specified servo.
 * * @param command A string in the format "S<ID>,<ANGLE>", e.g., "S1,90"
 */
void parseAndControlIndividualServo(String command) {
  // Remove any leading/trailing whitespace
  command.trim(); 
  
  // Simple validation check
  if (command.length() < 3 || command.charAt(0) != 'S') {
    Serial.println("ERROR: Invalid command format. Expected format: S<ID>,<ANGLE>");
    return;
  }

  // 1. Find the comma to split the string
  int commaIndex = command.indexOf(',');
  if (commaIndex == -1) {
    Serial.println("ERROR: Missing comma separator in command.");
    return;
  }
  
  // 2. Extract the Servo ID (S1, S2, S3, etc.)
  String idStr = command.substring(0, commaIndex); // Grab "S1" or "S2"
  
  // 3. Extract the Angle (after the comma)
  String angleStr = command.substring(commaIndex + 1);
  int servoAngle = angleStr.toInt(); // Convert the string angle to an integer

  // Check if the angle is valid
  if (servoAngle < 0 || servoAngle > 180) {
    Serial.printf("ERROR: Angle out of range (received %d, expected 0-180).\n", servoAngle);
    return;
  }

  // 4. CONTROL LOGIC: Use 'if/else if' to check the specific command string
  
  if (idStr.equals("S1")) {
    servo1.write(servoAngle);
    Serial.printf("✅ Executed command: %s -> Moving Servo 1 to %d degrees.\n", command.c_str(), servoAngle);
    
  } else if (idStr.equals("S2")) {
    servo2.write(servoAngle);
    Serial.printf("✅ Executed command: %s -> Moving Servo 2 to %d degrees.\n", command.c_str(), servoAngle);
    
  } else if (idStr.equals("S3")) {
    servo3.write(servoAngle);
    Serial.printf("✅ Executed command: %s -> Moving Servo 3 to %d degrees.\n", command.c_str(), servoAngle);
    
  } else if (idStr.equals("S4")) {
    servo4.write(servoAngle);
    Serial.printf("✅ Executed command: %s -> Moving Servo 4 to %d degrees.\n", command.c_str(), servoAngle);
    
  } else if (idStr.equals("S5")) {
    servo5.write(servoAngle);
    Serial.printf("✅ Executed command: %s -> Moving Servo 5 to %d degrees.\n", command.c_str(), servoAngle);
    
  } else {
    Serial.printf("ERROR: Servo ID not recognized (received %s, expected S1-S5).\n", idStr.c_str());
  }
}