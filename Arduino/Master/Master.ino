/*
* Master Program
* Created by us
*
*/
//Enums
typedef enum client {
  unknown = 'u',
  RaspberryPi = 'r',
  NXT = 'n',
  DEBUG = ' ',
  Arduino = 'a'
} client;

typedef struct {
  byte length;
  byte width;
  byte height;
} Shape;


typedef struct {
  byte length;
  byte width;
  byte height;
  byte colour;
} Package;

typedef struct {
  unsigned short sensor1;
  unsigned short sensor2;
  unsigned short sensor3;
  unsigned long time;
} SensorData;


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

Package packages[5];
SensorData sensorBuffer[100];

unsigned short lengthBetweenSensors = 0;
unsigned short heigthBetweenSensorAndBelt = 0;

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
		//RS485
		digitalWrite(SerialTransmitPin, LOW);

	delayMicroseconds(20);
}

/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
    
}
