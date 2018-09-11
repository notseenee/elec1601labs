#include <Servo.h>                      // Include servo library

Servo servoRight;                       // Declare left servo
Servo servoLeft;

int stopValue = 1510;

void setup() {
  servoRight.attach(13); 
  servoLeft.attach(12);  
}

void turnLeft(bool direction, int del) {
  int multi;
  if (direction) {
    multi = 1;
  } else {
    multi = -1;
  }
  servoRight.writeMicroseconds(stopValue - 200 * multi);
  servoLeft.writeMicroseconds(stopValue - 200 * multi);
  delay(del);
}

void forward(bool direction, int del) {
  int multi;
  if (direction) {
    multi = 1;
  } else {
    multi = -1;
  }
  servoRight.writeMicroseconds(stopValue - 200 * multi);
  servoLeft.writeMicroseconds(stopValue + 200 * multi);
  delay(del);
}

int turnTime = 1000;
void loop() {
    turnLeft(true, turnTime);
    turnLeft(false, turnTime);
}

