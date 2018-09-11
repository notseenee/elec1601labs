void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH); // Pin 13 = 5 V
  delay(20);             // for 0.2 seconds
  digitalWrite(13, LOW);  // Pin 13 = 0 V
  delay(10);            // for 1.8 seconds
}
