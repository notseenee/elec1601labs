#include <Servo.h> // Include servo library
#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 7
#define TxD 6

#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);

Servo servoRight; // Declare left servo
Servo servoLeft;
int ninetyDegreesTime = 542; //The value it takes for the arduino to turn 90 degrees
int oneMetreTime = 5500;
int stopValue = 1510; //A constant that is the stopped speed

void setup() {

	tone(4, 3000, 1000);
	delay(1000);
	pinMode(10, INPUT);  pinMode(9, OUTPUT);   // Left IR LED & Receiver


    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);

    setupBlueToothConnection();

	servoRight.attach(13);
	servoLeft.attach(12);
}



void loop() {
    char recvChar;
    while(1) {

		//check if there's any data sent from the remote bluetooth shield
        if(blueToothSerial.available()) {

            recvChar = blueToothSerial.read();

			if (recvChar == 'f') {
				if (!forward(true, oneMetreTime)) {
					turnLeft(true, ninetyDegreesTime * 2 );
				};
				stop(1500);
			} else {
				//
			}
            Serial.print(recvChar);

        }
		//check if there's any data sent from the local serial terminal, you can add the other applications here
        if(Serial.available()) {

            recvChar  = Serial.read();
            blueToothSerial.print(recvChar);

        }

    }

}



void setupBlueToothConnection()
{
    blueToothSerial.begin(38400);                           // Set BluetoothBee BaudRate to default baud rate 38400
    blueToothSerial.print("\r\n+STWMOD=0\r\n");             // set the bluetooth work in slave mode
    blueToothSerial.print("\r\n+STNA=Slave2+\r\n");    // set the bluetooth name as "SeeedBTSlave"
    blueToothSerial.print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect me
    blueToothSerial.print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here
    delay(2000);                                            // This delay is required.
    blueToothSerial.print("\r\n+INQ=1\r\n");                // make the slave bluetooth inquirable
    Serial.println("The slave bluetooth is inquirable!");
    delay(2000);                                            // This delay is required.

    blueToothSerial.flush();

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
bool forward(bool direction, int del) {
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
		return true;
	} else {
		return false;
	}
}

void stop(int del) {
	servoLeft.writeMicroseconds(stopValue);
	servoRight.writeMicroseconds(stopValue);
	delay(del);
}

bool delayCheckCollision(int del) {
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

bool checkCollision() {
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
