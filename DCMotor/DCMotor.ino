int adder = 5;
int value = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(9, HIGH);
  value += adder;
  analogWrite(9, value);
  Serial.println(value % 255);
  delay(200);
}
