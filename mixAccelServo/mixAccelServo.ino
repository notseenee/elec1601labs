#include <Servo.h>
int timer = 0;
int angle = 0;

Servo wiper;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wiper.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  float x = offset(analogRead(0));
  float y = offset(analogRead(1));
  float z = offset(analogRead(2));

  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);

  if ((abs(x) - 0.2 < 0) && (abs(y) - 0.2 < 0) && (abs(z) - 0.7) < 0) {
    if (angle < 180) {
      angle += 1;
    }
  } else {
    angle = 0;
  }

  wiper.write(angle);
  
  delay(10);
}

float offset(float value) {
  float xvolt = (float) value * 5.0/1024.0;
  return (xvolt - 1.65)/0.8;
}

