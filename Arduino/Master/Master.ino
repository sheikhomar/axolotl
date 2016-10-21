/*
* Master Program
* Created by us
*
*/
//Enums
typedef enum client {
  none = 0,
  unknown = 'u',
  RaspberryPi = 'r',
  NXT = 'n',
  DEBUG = ' ',
  Arduino = 'a'
} client;


//Pins 
	//LEDs
	#define LED1 8
	#define LED2 9
	#define LED3 10
	//Ult Sensors
	#define ult1_TrigPin 2
	#define ult1_echoPin 3
	#define ult2_TrigPin 4
	#define ult2_echoPin 5
	#define ult3_TrigPin 6
	#define ult3_echoPin 7
	//Rs485
	#define SerialTransmitPin 12

//Values
	#define ultMmPerUS 0.170145 //based on speed of sound * 0.5
	#define ultMeasurementCycle 60 //60 ms, from manual
	#define ultTrigPulse 10 //uS, from manual
	#define baud 57600
	#define serialMaxDataSize 12

//Global variables



/***************************
setup

Initiate all pins on the Arduino.
***************************/
void setup() {
	//Initiate Serial Communication
	Serial.begin(baud);

	//Initiate Pins
		//LEDs
		pinMode(LED1, OUTPUT);
		pinMode(LED2, OUTPUT);
		pinMode(LED3, OUTPUT);
		//Ult Sensors
		pinMode(ult1_TrigPin, OUTPUT);
		pinMode(ult1_echoPin, INPUT);
		pinMode(ult2_TrigPin, OUTPUT);
		pinMode(ult2_echoPin, INPUT);
		pinMode(ult3_TrigPin, OUTPUT);
		pinMode(ult3_echoPin, INPUT);

	//Set default values of pins
		//LEDs
		digitalWrite(LED1, LOW);
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, LOW);
		//Ult Sensors
		digitalWrite(ult1_TrigPin, LOW);
		digitalWrite(ult2_TrigPin, LOW);
		digitalWrite(ult2_TrigPin, LOW);

	delayMicroseconds(20);
}


/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
    byte data[100];
    data[0] = 5;
    serialSendData(NXT, data, 0, 5);
    client whichClient = serialCheck();
    if (whichClient == NXT)
    {
        serialReadData(data, 100);
    }

	//check serial
	//if time to push lego
	//if handshake / message for me
	//answer
	//if bussy wait a few iterations before we read the buffer
	//read colour

    while(1);
}


bool tag() {
	bool sensor1, sensor2, sensor3;
	double distance;

	distance = GetUltDistance(ult1_TrigPin, ult1_echoPin, false);
	sensor1 = distance < 45;

	distance = GetUltDistance(ult2_TrigPin, ult2_echoPin, false);
	sensor2 = distance < 60;

	distance = GetUltDistance(ult3_TrigPin, ult3_echoPin, false);
	sensor3 = distance < 33;

	return (sensor1 && sensor2);
}
