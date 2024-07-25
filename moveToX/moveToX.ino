
// Constants for pin numbers
const int PIN_ENABLE = 2;
const int PIN_STEP = 3;

// Distance per step in millimeters
const float STEPS_PER_MM = 5200.0/171.45;
float oldDist;
// Initialize the current position variable
float currentX = 0;
int curr_pos = 0;

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

  // Print instructions to the serial monitor
  //Serial.println("Enter distance in millimeters to move the stepper motor:");
  digitalWrite(PIN_ENABLE, LOW);
  // for (int i = 0; i < 4800; i++) {
  //   digitalWrite(PIN_STEP, HIGH);
  //   delayMicroseconds(1250); // Adjust the speed as needed
  //   digitalWrite(PIN_STEP, LOW);
  //   delayMicroseconds(1250); // Adjust the speed as needed
  // }
  // digitalWrite(PIN_ENABLE, HIGH);
  // for (int i = 0; i < 2400; i++) {
  //   digitalWrite(PIN_STEP, HIGH);
  //   delayMicroseconds(1250); // Adjust the speed as needed
  //   digitalWrite(PIN_STEP, LOW);
  //   delayMicroseconds(1250); // Adjust the speed as needed
  // }
  curr_pos = 0;
}

// void loop() {
//   if (Serial.available() > 0) {
//     // Read the input from the serial as a string
//     lengthText = Serial.readStringUntil('\n');
//     if (lengthText == 'calibrate') {
//       calibrate(oldDist);
//     }
//     // Convert the length from text to a float
//     float length = lengthText.toFloat();

//     // Move to the specified length
//     moveToX(length);

//     // Print the current position to the serial monitor
//     Serial.print("Moved to: ");
//     Serial.print(currentX);
//     Serial.println(" mm");
//   }
// }

void loop() {
  delay(100);
  float coordinates[3];
  if (Serial.available() > 0) {
    // Read the input from the serial as a string
    lengthText = Serial.readStringUntil('\n');
    lengthText.trim();
    // if (lengthText == "calibrate") {
    //   calibrate(oldDist);
    // } else {
      // Parse the string into individual floats
      int index = 0;
      char *token = strtok((char *)lengthText.c_str(), ", ");
      coordinates[index] = atof(token);
      index++;
      do {
        token = strtok(NULL, ", ");
        coordinates[index] = atof(token);
        //Serial.println(token);
        index++;
      } while (index < 3);
        // Serial.println(token);
        // coordinates[index] = atof(token);
        // token = strtok(NULL, ", ");
        // Serial.println(token);

        // index++;

      // // Convert the length from text to a float
      // float length = lengthText.toFloat();

      // // Move to the specified length
      // moveToX(length);

      // // Print the current position to the serial monitor
      // Serial.println(coordinates[0]);
      // Serial.println(coordinates[1]);
      // Serial.println(coordinates[2]);
      // Serial.print(currentX);
      // Serial.println(" mm");
    // }
  } 
  int coords = coordinates[2] * 10;
  int step_pos = coords * 240;
  Serial.println(step_pos);
  if (step_pos < 0) {
    step_pos = 0;
  } else if (step_pos > 4800) {
    step_pos = 4800;
  }

  // update position, maxing at 200 steps per cycle 
  Serial.print("Curr: ");
  Serial.println(curr_pos);
  Serial.print("exp: ");
  Serial.println(step_pos); 
  if (step_pos - curr_pos > 200) {
    digitalWrite(PIN_ENABLE, HIGH);
    curr_pos += 200;
  } else if (step_pos - curr_pos < -200) {
    curr_pos -= 200;
    digitalWrite(PIN_ENABLE, LOW);
  } else {
    return;
  }
  for (int i = 0; i < 200; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(1250); // Adjust the speed as needed
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(1250); // Adjust the speed as needed
  }
}

// void moveToX(float x) {

//   // 171.45/4800
//   // 171.45 is the length of the gantry axis 
//   // Calculate the number of steps to move
//   // int stepsToMove = round((x - currentX) / DISTANCE_PER_STEP);

//   // Take the dist / 

//   float dist = x/171.45;
//   int stepsToMove = 5200*dist;
//   // Determine the direction of movement
//   bool direction = (stepsToMove > 0);
//   // 5200 STEPS ACROSS THE WHOLE GANTRY
//   // 171.45 IS THE DIST ACROSS THE GANTRY
//   // 5200/171.45
//   // Enable the motor driver
//   if (dist > 0.0) { // positive is towards the motor
//     digitalWrite(PIN_ENABLE, HIGH);
//   } else { // negative is away from the motor 
//     digitalWrite(PIN_ENABLE, LOW);
//   }
//   // HIGH IS TOWARDS THE MOTOR 
//   // Move the motor the required number of steps
//   for (int i = 0; i < abs(stepsToMove); i++) {
//     digitalWrite(PIN_STEP, HIGH);
//     delayMicroseconds(1250); // Adjust the speed as needed
//     digitalWrite(PIN_STEP, LOW);
//     delayMicroseconds(1250); // Adjust the speed as needed
//   }

//   //HOW TO CALIBRATE

//   // Update the current position
//   currentX = x;

//   // Disable the motor driver
//   digitalWrite(PIN_ENABLE, HIGH);
//   oldDist = x;
// }

// void calibrate(float prevDist) {
//   float dist = abs(prevDist - 171.45);
//   int stepsToMove = 5200*dist;
//   // Determine the direction of movement
//   bool direction = (stepsToMove > 0);
//   // 5200 STEPS ACROSS THE WHOLE GANTRY
//   // 171.45 IS THE DIST ACROSS THE GANTRY
//   // 5200/171.45
//   // Enable the motor driver
//   if (dist > 0.0) { // positive is towards the motor
//     digitalWrite(PIN_ENABLE, HIGH);
//   } else { // negative is away from the motor 
//     digitalWrite(PIN_ENABLE, LOW);
//   }
//   for (int i = 0; i < abs(stepsToMove); i++) {
//     digitalWrite(PIN_STEP, HIGH);
//     delayMicroseconds(1250); // Adjust the speed as needed
//     digitalWrite(PIN_STEP, LOW);
//     delayMicroseconds(1250); // Adjust the speed as needed
//   }
// }

// void calibrate() {
//   // Move to the start of the gantry
//   int stepsToMove = 5200; // Move all the way to one side of the gantry

//   // Enable the motor driver
//   digitalWrite(PIN_ENABLE, LOW);

//   // Move the motor the required number of steps
//   for (int i = 0; i < stepsToMove; i++) {
//     digitalWrite(PIN_STEP, HIGH);
//     delayMicroseconds(1250); // Adjust the speed as needed
//     digitalWrite(PIN_STEP, LOW);
//     delayMicroseconds(1250); // Adjust the speed as needed
//   }

//   // Update the current position
//   currentX = 0; // Reset the current position to 0

//   // Disable the motor driver
//   digitalWrite(PIN_ENABLE, HIGH);

//   Serial.println("Calibration complete. Position reset to 0 mm.");
// }

// void calibrate(float prevDist) {
//   // Move back to the start of the gantry
//   float distToMove = prevDist; // Distance to move back to the start
//   int stepsToMove = round(distToMove * STEPS_PER_MM);

//   // Enable the motor driver
//   digitalWrite(PIN_ENABLE, LOW);

//   // Move the motor the required number of steps
//   for (int i = 0; i < abs(stepsToMove); i++) {
//     digitalWrite(PIN_STEP, HIGH);
//     delayMicroseconds(1250); // Adjust the speed as needed
//     digitalWrite(PIN_STEP, LOW);
//     delayMicroseconds(1250); // Adjust the speed as needed
//   }

//   // Update the current position
//   currentX = 0; // Reset the current position to 0

//   // Disable the motor driver
//   digitalWrite(PIN_ENABLE, HIGH);

//   Serial.println("Calibration complete. Position reset to 0 mm.");
// }