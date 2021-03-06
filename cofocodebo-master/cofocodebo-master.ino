/*
Lab T15 Group 2
Codebo Master Code

  functions: 
    bool checkForCommand(char recvChar) - parse special characters to trigger commands
    void rgb(char col) - display different colours on the LED and beep
    
  All other functions and variables are adapted from the Bluetooth mastercode from Canvas.

*/

// from Canvas Bluetooth master code
#include <SoftwareSerial.h>        // Software Serial Port
#define RxD 7
#define TxD 6
#define DEBUG_ENABLED  1

String retSymb = "+RTINQ=";        // start symbol when there's any return
String slaveName = ";Slave18";     // caution that ';'must be included, and make sure the slave name is right.
int nameIndex = 0;
int addrIndex = 0;

String recvBuf;
String slaveAddr;
String connectCmd = "\r\n+CONN=";
SoftwareSerial blueToothSerial(RxD,TxD);


void setup() {
    Serial.begin(9600);
  
    // Set LED pins
    pinMode(9, OUTPUT); //Red
    pinMode(10, OUTPUT); //Green
    pinMode(11, OUTPUT); //Blue
    // Play startup tone
    tone(13, 3000, 1000);
    delay(1000);

    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);

    setupBlueToothConnection();

    //wait 1s and flush the serial buffer
    delay(1000);

    Serial.flush();
    blueToothSerial.flush();

}

void loop() {
    char recvChar;

    while(1) {
    //check if there's any data sent from the remote bluetooth shield
        if(blueToothSerial.available()) {
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
      
            checkForCommand(recvChar);
        }
    
    //check if there's any data sent from the local serial terminal, you can add the other applications here
        if(Serial.available()) {
            recvChar  = Serial.read();
            blueToothSerial.print(recvChar);
      
            checkForCommand(recvChar);
        }
    }
}

// Parse commands from Bluetooth or local serials
bool checkForCommand(char recvChar) {
  // ALL POSSIBLE COMMANDS FROM THE SLAVE:
    // ` - en route
    // ~ - collision
    // @ - returning
    // # - delivery complete
    // & - stopped

  if (recvChar == '`' || recvChar == '~' ||
    recvChar == '@' || recvChar == '#' || recvChar == '&') {
    rgb(recvChar);
  }
}

// Change LED colour and play tone based on character command
void rgb(char col) {
  switch (col) {
    case '~': // red
      analogWrite(9, 255);
      analogWrite(10, 0);
      analogWrite(11, 0);
      break;
    case '#': // green
      analogWrite(9, 0);
      analogWrite(10, 255);
      analogWrite(11, 0);
      break;
    case '`': // blue
      analogWrite(9, 0);
      analogWrite(10, 0);
      analogWrite(11, 255);
      break;
    case '@': // white
      analogWrite(9, 255);
      analogWrite(10, 255);
      analogWrite(11, 255);
      break;
    case '&': // stopped
      analogWrite(9, 0);
      analogWrite(10, 0);
      analogWrite(11, 0);
      break;
  }
  
  tone(13, 3000, 500);
  delay(500);
}

void setupBlueToothConnection()
{
    blueToothSerial.begin(38400);                               // Set BluetoothBee BaudRate to default baud rate 38400
    blueToothSerial.print("\r\n+STWMOD=1\r\n");                 // set the bluetooth work in master mode
    
    blueToothSerial.print("\r\n+STNA=Master18\r\n");       // set the bluetooth name as
        
    blueToothSerial.print("\r\n+STAUTO=0\r\n");                 // Auto-connection is forbidden here
    delay(2000);                                                // This delay is required.
    blueToothSerial.flush();
    blueToothSerial.print("\r\n+INQ=1\r\n");                    //make the master inquire
    Serial.println("Master is inquiring!");
    delay(2000); // This delay is required.



    //find the target slave

    char recvChar;
    while(1) {
        if(blueToothSerial.available()) {
            recvChar = blueToothSerial.read();
            recvBuf += recvChar;
            nameIndex = recvBuf.indexOf(slaveName);             //get the position of slave name
            //decrease the ';' in front of the slave name, to get the position of the end of the slave address
            if ( nameIndex != -1 ) {
                //Serial.print(recvBuf);
                addrIndex = (recvBuf.indexOf(retSymb,(nameIndex - retSymb.length()- 18) ) + retSymb.length());//get the start position of slave address
                slaveAddr = recvBuf.substring(addrIndex, nameIndex);//get the string of slave address
                break;
            }
        }
    }
    //form the full connection command
    connectCmd += slaveAddr;
    connectCmd += "\r\n";
    int connectOK = 0;
    Serial.print("Connecting to slave:");
    Serial.print(slaveAddr);
    Serial.println(slaveName);
    //connecting the slave till they are connected
    do {
        blueToothSerial.print(connectCmd);//send connection command
        recvBuf = "";
        while(1) {
            if(blueToothSerial.available()) {
                recvChar = blueToothSerial.read();
                recvBuf += recvChar;
                if(recvBuf.indexOf("CONNECT:OK") != -1) {
                    connectOK = 1;
                    Serial.println("Connected!");
                    blueToothSerial.print("Connected!");
                    break;
                } else if(recvBuf.indexOf("CONNECT:FAIL") != -1) {
                    Serial.println("Connect again!");
                    break;
                }
            }
        }
    } while(0 == connectOK);
}
