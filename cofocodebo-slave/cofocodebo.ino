#include <Servo.h> // Include servo library
#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 7
#define TxD 6

#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);

Servo servoRight; // Declare left servo
Servo servoLeft;
long ninetyDegreesTime = 542L; //The value it takes for the arduino to turn 90 degrees
long oneMetreTime = 5800L;//6500;
long stopValue = 1510L; //A constant that is the stopped speed


void setup() {

	tone(4, 3000, 1000);
	delay(1000);
	pinMode(10, INPUT);  pinMode(9, OUTPUT);   // Left IR LED & Receiver
	pinMode(3, INPUT);  pinMode(2, OUTPUT);   // Left IR LED & Receiver


    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);

    setupBlueToothConnection();

	servoRight.attach(13);
	servoLeft.attach(12);

	blueToothSerial.print("@");
}

void loop() {
    char revChar;
    while(1) {
		//check if there's any data sent from the remote bluetooth shield
        if (blueToothSerial.available()) {
            revChar = blueToothSerial.read();
			checkForCommand(revChar);
        }
		//check if there's any data sent from the local serial terminal, you can add the other applications here
        if (Serial.available()) {
            revChar = Serial.read();
            blueToothSerial.print(revChar);
        }

		stop(100);
    }
}

bool goToCoord = false;
long xC = -1;
long yC = -1;

bool checkForCommand(char revChar) {
	if (revChar == '(' && !goToCoord) {
		goToCoord = true;
		return false;
	}

	if (goToCoord) {
		if (xC == -1) {
			xC = long(revChar) - 48;
			return false;
		} else if (yC == -1) {
			yC = long(revChar) - 48;
			return false;
		} else if (revChar == ')') {
			//Do Stuff Here
			Serial.println("\nGoing to: " + String(xC) + ", " + String(yC));
			followCoordinate(xC, yC);
			goToCoord = false;
			xC = -1;
			yC = -1;
		}
	}
}

long detectRate = 100;

void followCoordinate(long x, long y) {
	long xDel = x * oneMetreTime;
	long yDel = y * oneMetreTime;
	long allowed_fails = 6L;
	long fails = 0L;
	bool moving = true;
	bool nextIsLeft = true;
	bool failed = false;
	long new_del = 0;

	//Enroute
	//Main calculation
	while (true) {
		//Movement
		if (nextIsLeft && xDel > 0) {
			//xAxis
			new_del = forward(true, xDel);
			xDel = xDel - new_del;
			Serial.println("xDel: " + (String)xDel);

		} else if (yDel > 0) {
			//yAxis
			new_del = forward(true, yDel);
			yDel = yDel - new_del;
			Serial.println("yDel: " + (String)yDel);
		} else {
			break;
		}

		if ((xDel > 0 && nextIsLeft) || (yDel > 0 && !nextIsLeft)) {
			fails += 1;
		}

		if (fails >= allowed_fails) {
			failed = true;
			break;
		}

		turnLeft(nextIsLeft, ninetyDegreesTime);
		nextIsLeft = !nextIsLeft;
	}

	if (failed) {
		blueToothSerial.print("Failed Movement to " + (String)x + ", " + (String)y + ".\n");
		Serial.println("Fail");
		blueToothSerial.print("~");
	} else {
		blueToothSerial.print("Successfully moved to " + (String)x + ", " + (String)y + ".\n");
		Serial.println("At destination: " + (String)x + ", " + (String)y);
		blueToothSerial.print("#");
	}
}


void setupBlueToothConnection()
{
    blueToothSerial.begin(38400);                           // Set BluetoothBee BaudRate to default baud rate 38400
    blueToothSerial.print("\r\n+STWMOD=0\r\n");             // set the bluetooth work in slave mode
    blueToothSerial.print("\r\n+STNA=Slave11+\r\n");   		// set the bluetooth name as "SeeedBTSlave"
    blueToothSerial.print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect me
    blueToothSerial.print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here
    delay(2000);                                            // This delay is required.
    blueToothSerial.print("\r\n+INQ=1\r\n");                // make the slave bluetooth inquirable
    Serial.println("The slave bluetooth is inquirable!");
    delay(2000);                                            // This delay is required.

    blueToothSerial.flush();

}

// IR Object Detection Function
long irDetect(long irLedPin, long irReceiverPin, long frequency) {
	tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
	delay(1);                                  // Wait 1 ms
	long ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
	delay(1);                                  // Down time before recheck
	return ir;                                 // Return 1 no detect, 0 detect
}

//Controls Left/Right Movement
void turnLeft(bool direction, long del) {
	long multi;
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
//Returns the delay still to be done because it failed.
long forward(bool direction, long del) {
	long multi;
	if (direction) {
		multi = 1;
	} else {
		multi = -1;
	}
	blueToothSerial.print("`");
	servoRight.writeMicroseconds(stopValue + 200 * multi);
	servoLeft.writeMicroseconds(stopValue - 200 * multi);
	return delayCheckCollision(del);
}

void stop(long del) {
	servoLeft.writeMicroseconds(stopValue);
	servoRight.writeMicroseconds(stopValue);
	delay(del);
}

long delayCheckCollision(long del) {
	long amount = del / detectRate;
	long rem = del % detectRate;
	for (int i = 0; i < amount; i++) {
		if (checkCollision()) {
			//The remainder of the delay
			return (i * detectRate + rem);
		}
		Serial.println("Collision Detections Left: " + String(amount - i));
		delay(detectRate);
	}
	if (checkCollision()) {
		return rem;
	}
	delay(rem);
	return del;
}

bool checkCollision() {
	//4ms delay looking for ir.
	int irEyeLeft = !irDetect(9, 10, 38000);
	int irEyeRight = !irDetect(2, 3, 38000);
	//Serial.println("Eyes: " + (String)irEyeLeft + ", " + (String)irEyeRight);
	if (irEyeLeft || irEyeRight)
	{
		Serial.println("Collision Ahead detected");
		blueToothSerial.print("~");
		blueToothSerial.println("Collision Ahead detected");
		return true;
	} else {
		return false;
	}
}