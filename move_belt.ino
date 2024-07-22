const int stepPin = 3; // Pin to send step pulses
const int dirPin = 2;  // Pin to set direction or enable
const float distancePerStepInches = 0.2925; // Distance the belt moves per step in inches
const float inchToMm = 25.4; // Conversion factor from inches to millimeters

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600); // Start the Serial Monitor with a baud rate of 9600
  Serial.println("Enter the distance to move (in mm):"); // Prompt user to enter distance
}

void moveBelt(float distanceMm) {
  float distanceInches = distanceMm / inchToMm;
  
  // Calculate the number of steps needed to move the desired distance in inches
  int steps = distanceInches / distancePerStepInches;
  // circumfrence is 58.5 inches 
  if (distanceMm > 0) {
    digitalWrite(dirPin, HIGH); // Set direction to forward
  } else {
    digitalWrite(dirPin, LOW); // Set direction to backward
    steps = -steps; // Convert steps to positive since distance is negative
  }

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000); // we have 1.8 degrees moved per step - 200 steps per rev
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000); 

    // Print the current step and distance moved to the Serial Monitor
    float currentDistanceInches = (i + 1) * distancePerStepInches;
    float currentDistanceMm = currentDistanceInches * inchToMm;
    Serial.print("Step: ");
    Serial.print(i + 1);
    Serial.print(" - Distance: ");
    Serial.print(currentDistanceMm);
    Serial.println(" mm");
  }
}

void loop() {
  // Checking Serial Monitor
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read the input from Serial Monitor
    float distanceMm = input.toFloat(); // Convert the input to a float

    // Move the belt the specified distance in mm
    moveBelt(distanceMm);

    // Prompt the user to enter the distance again
    Serial.println("Enter the distance to move (in mm):");
  }
}
