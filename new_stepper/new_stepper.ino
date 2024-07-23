void setup() {
  // Initialize pin 8 as an output
  pinMode(2, OUTPUT);

  // Initialize pin 10 as an output
  pinMode(3, OUTPUT);

}

void loop() {
  // Set pin 8 high (turn it on)
  digitalWrite(2, LOW);
  for(int i = 0; i < 3*8*200; i++ ) {
    digitalWrite(3, LOW);
    digitalWrite(3, HIGH);
    //delayMicroseconds(1250); 
    delay(2);
  }
  delay(1000);
  digitalWrite(2, HIGH);
  for(int i = 0; i < 3*8*200; i++ ) {
    digitalWrite(3, LOW);
    digitalWrite(3, HIGH);
    //delayMicroseconds(1250); 
    delay(2);
  }
  delay(1000);
  //digitalWrite(2, HIGH);
  //   for(int i = 0; i < 600; i++ ) {
  //   digitalWrite(3, LOW);
  //   digitalWrite(3, HIGH);
  //   //delayMicroseconds(1250); 
  //   delay(4);
  // }
}