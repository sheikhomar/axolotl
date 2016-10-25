#define SENSOR_BUFFER_SIZE 100
#define PACKAGE_BUFFER_SIZE 10

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

unsigned short lengthBetweenSensors = 0;
unsigned short heigthBetweenSensorAndBelt = 0;

void runConveyorBeltAtSpeed(byte speed) {
   byte data[1] = { speed };
   serialSendData(NXT, data, 1, 3);
}

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

  runConveyorBeltAtSpeed(50);

  delayMicroseconds(20); 
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

void simplifiedDemo() {
    //Test code
  byte dataD[] = { '.' };
  byte dataS[] = { 50 };
  bool ta = false;
  bool da;

  serialSendData(NXT, dataS, 0, 3);
  serialSendData(NXT, dataS, 1, 6);
  delay(1000);
  serialSendData(NXT, dataS, 1, 6);

  while (true) {
    if ((ta = tag()) && da) {
      serialSendData(DEBUG, dataD, 2, 10);
      delay(1900);
      serialSendData(NXT, dataS, 1, 0);
    }
    else {
      delay(25);
    }
    da = ta;
  }
}

/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
  Package packages[PACKAGE_BUFFER_SIZE];
  unsigned short packageStartIndex = 0;
  unsigned short packageEndIndex = 0;

  SensorData sensorBuffer[SENSOR_BUFFER_SIZE];
  unsigned short sensorBufferStartIndex = 0;
  unsigned short sensorBufferEndIndex = 0;
  
  while (true) {
    bool newPackageDetected = readSensors(sensorBuffer, sensorBufferEndIndex);
    if (newPackageDetected) {
      // We have detected a new package in the conveyor belt

      sensorBufferEndIndex++;
    } else {
      
      if (sensorBufferStartIndex != sensorBufferEndIndex) {
        // At this stage we have collected distance information for a single package.
        // The function 'handleSensorData' builds an instance of Package based 
        // on the data in the sensorData.
        handleSensorData(&packages[packageEndIndex], 
          sensorBuffer, sensorBufferStartIndex, sensorBufferEndIndex);

        Package p = packages[packageEndIndex];
        String t2 = String(p.width);
        serialDebug("Package: " + String(p.width) + " x " + String(p.height) + " x " + String(p.length) + "\n");
      }
    }
  }
}
