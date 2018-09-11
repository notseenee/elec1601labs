int del = 500;

void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT); //Red
  pinMode(10, OUTPUT); //Green
  pinMode(11, OUTPUT); //Blue
}

void loop() {
  // put your main code here, to run repeatedly:
  rgb(255, 0, 0); //Red
  rgb(255, 40, 0); //Orange
  rgb(255, 128, 0); //Yellow
  rgb(0, 255, 0); //Green
  rgb(0, 255, 255); //LBlue
  rgb(0, 0, 255); //Blue
  rgb(128, 0, 255); //Purple
}

void rgb(int red, int green, int blue) {
  analogWrite(9, red);
  analogWrite(10, green);
  analogWrite(11, blue);
  delay(del);
}

