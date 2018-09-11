#include <Servo.h>

Servo spinBoi;

void setup() {
  // put your setup code here, to run once:
  spinBoi.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  spinBoi.write(1);
  delay(400);
  spinBoi.write(159);
  delay(400);
}
