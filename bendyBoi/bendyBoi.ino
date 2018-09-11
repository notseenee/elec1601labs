float con = 4.02;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int pot_value = analogRead(0)/con;
  Serial.println(255 - pot_value);
  analogWrite(9, 255 - pot_value);
  
  delay(100);
  
}
