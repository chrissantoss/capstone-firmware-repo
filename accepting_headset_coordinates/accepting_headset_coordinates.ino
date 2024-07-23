// Constants for pin numbers
const int PIN_ENABLE = 2;
const int PIN_STEP = 3;

// Number of steps per millimeter
const float STEPS_PER_MM = 4.8;

// Initialize the current position variable
float currentX = 0;

// Length in millimeters as text (string)
String lengthText = "";

void setup() {
  // Initialize pin 2 as an output
  pinMode(PIN_ENABLE, OUTPUT);

  // Initialize pin 3 as an output
  pinMode(PIN_STEP, OUTPUT);

  // Enable the motor driver
  digitalWrite(PIN_ENABLE, LOW);

  // Begin serial communication
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the input from the serial as a string
    lengthText = Serial.readStringUntil('\n');

    // Convert the length from text to a float
    float length = lengthText.toFloat();

    // Move to the specified length
    moveToX(length);

    // Delay to observe the movement
    delay(5000);

    // Example usage: move back to 0 mm
    moveToX(0);

    // Delay to observe the movement
    delay(5000);
  }
}

void moveToX(float x) {
  // Calculate the number of steps to move
  int stepsToMove = round((x - currentX) * STEPS_PER_MM);

  // Determine the direction of movement
  bool direction = (stepsToMove > 0);

  // Enable the motor driver
  digitalWrite(PIN_ENABLE, LOW);

  // Move the motor the required number of steps
  for (int i = 0; i < abs(stepsToMove); i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(1250); // Adjust the speed as needed
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(1250); // Adjust the speed as needed
  }

  // Update the current position
  currentX = x;

  // Disable the motor driver
  digitalWrite(PIN_ENABLE, HIGH);
}
