#define BUTTON_PIN 6
const int stepPin = 3;
const int dirPin = 2;
const float distancePerStepInches = 0.2925;
const float inchToMm = 25.3;

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}
// going to try to record the distance travelled and the nunmber of steps used up within given button clicks 
// ie. click a button to begin, then click button again to stop and record the number of steps travelled 
void loop()
{
  byte buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH) {
      Serial.println("Button is pressed");
  }
  else {
      // Serial.println("Button is not pressed");
  }
  delay(1);
}

// float recordStepCnt() {

// }