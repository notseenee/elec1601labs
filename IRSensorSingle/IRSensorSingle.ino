#include <Servo.h>                      // Include servo library

/*
 * Robotics with the BOE Shield - TestLeftIR
 * Display 1 if the left IR detector does not detect an object,
 * or 0 if it does.
 *
*/

//Movement
Servo servoRight; // Declare left servo
Servo servoLeft;
int ninetyDegreesTime = 542; //The value it takes for the arduino to turn 90 degrees
int oneMetreTime = 5500;
int stopValue = 1510; //A constant that is the stopped speed

void setup() {
	//IR Sensor Initialisation
	tone(4, 3000, 1000);                       // Play tone for 1 second
	delay(1000);                               // Delay to finish tone
	pinMode(10, INPUT);  pinMode(9, OUTPUT);   // Left IR LED & Receiver
	Serial.begin(9600);                        // Set data rate to 9600 bps

	//Movement Servos Initialisation
	servoRight.attach(13);
	servoLeft.attach(12);
}

void loop() {
	//Simple zigzag
	forward(true, oneMetreTime);
	stop(1500);                                // Stop for 1.5 seconds
}

// IR Object Detection Function
int irDetect(int irLedPin, int irReceiverPin, long frequency) {
	tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
	delay(1);                                  // Wait 1 ms
	int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
	delay(1);                                  // Down time before recheck
	return ir;                                 // Return 1 no detect, 0 detect
}

//Controls Left/Right Movement
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

//Controls Forward/Back Movement
void forward(bool direction, int del) {
	int multi;
	if (direction) {
		multi = 1;
	} else {
		multi = -1;
	}


	servoRight.writeMicroseconds(stopValue + 200 * multi);
	servoLeft.writeMicroseconds(stopValue - 200 * multi);
	if (delayCheckCollision(del)) {
		//Do next command
		Serial.println("Successfully moved forward");
	}
}

void stop(int del) {
	servoLeft.writeMicroseconds(stopValue);
	servoRight.writeMicroseconds(stopValue);
	delay(del);
}

boolean delayCheckCollision(int del) {
	int amount = del / 100;
	int rem = del % 100;
	for (int i = 0; i < amount; i++) {
		if (checkCollision()) {
			return false;
		}
		delay(100);
	}
	if (checkCollision()) {
		return false;
	}
	delay(rem);
	return true;
}

boolean checkCollision() {
	int irEyes = !irDetect(9, 10, 38000);
	Serial.println("Eyes: " + (String)irEyes);
	if (irEyes)
	{
		Serial.println("Collision Ahead detected");
		return true;
	} else {
		return false;
	}
}