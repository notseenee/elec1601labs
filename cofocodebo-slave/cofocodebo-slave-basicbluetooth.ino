#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 7
#define TxD 6

#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);



void setup() {

    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);

    setupBlueToothConnection();
}



void loop() {
    char recvChar;
    while(1) {

		//check if there's any data sent from the remote bluetooth shield
        if(blueToothSerial.available()) {

            recvChar = blueToothSerial.read();
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
