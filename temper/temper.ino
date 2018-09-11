float mvolts;
int units;
float temp;
int del = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(11, OUTPUT); //Red
  pinMode(10, OUTPUT); //Green
  pinMode(9, OUTPUT); //Blue

  //High 27
  //Low 22
}

void loop() {
  // put your main code here, to run repeatedly:
  units = analogRead(0);
  //Serial.println(units);
  mvolts = 4.88 * units;
  temp = mvolts/10 - 63;
  
  Serial.println(temp);

  int red = ((temp - 25) * 20)*256;
  
  rgb(red, 0, 0);
}

void rgb(int red, int green, int blue) {
  analogWrite(11, red);
  analogWrite(10, green);
  analogWrite(9, blue);
  delay(del);
}
