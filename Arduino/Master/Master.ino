/*
* Master Program
*
*/

//Includes
#include <SoftwareSerial.h>


//Enums
typedef enum client {
  unknown = 'u',
  RaspberryPi = 'r',
  NXT = 'n',
  DEBUG = ' ',
  Arduino = 'a'
} client;

typedef struct {
    unsigned short length;
    unsigned short width;
    unsigned short height;
    byte colour;
    unsigned long middleTime;
    bool isHandled;
} Package;

typedef struct {
    unsigned short sensor1;
    unsigned short sensor2;
    unsigned short sensor3;
    unsigned long time;
} SensorData;

//Pins 
  //LEDs
  #define LED1_PIN 8
  #define LED2_PIN 9
  #define LED3_PIN 10
  //add built-in LED todo

  //Ult Sensors
  #define ULT1_TRIG_PIN 2
  #define ULT1_ECHO_PIN 3
  #define ULT2_TRIG_PIN 4
  #define ULT2_ECHO_PIN 5
  #define ULT3_TRIG_PIN 6
  #define ULT3_ECHO_PIN 7
  //Rs485
  #define RS485_TX 10
  #define RS485_RX 11
  #define SERIAL_TRANSMIT_PIN 13

//Constants
  #define ULT_MM_PER_US 0.170145 //based on speed of sound * 0.5
  #define ULT_MEASUREMENT_CYCLE 60 //60 ms, from manual
  #define ULT_TRIG_PULSE 10 //uS, from manual


//Program variables
  #define BAUD 57600
  #define BAUD_DEBUG 9600
  #define SERIAL_MAX_DATA_SIZE 12

//Global variables
unsigned short lengthBetweenSensors = 0;
unsigned short heigthBetweenSensorAndBelt = 0;

//Objects
SoftwareSerial RS485Serial(RS485_RX, RS485_TX);



/***************************
setup

Initiate all pins on the Arduino.
***************************/
void setup() {
  //Initiate Serial Communication
    Serial.begin(BAUD_DEBUG);
    RS485Serial.begin(BAUD);

  //Initiate Pins
    //LEDs
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);
    //Ult Sensors
    pinMode(ULT1_TRIG_PIN, OUTPUT);
    pinMode(ULT1_ECHO_PIN, INPUT);
    pinMode(ULT2_TRIG_PIN, OUTPUT);
    pinMode(ULT2_ECHO_PIN, INPUT);
    pinMode(ULT3_TRIG_PIN, OUTPUT);
    pinMode(ULT3_ECHO_PIN, INPUT);
    //RS485
    pinMode(SERIAL_TRANSMIT_PIN, OUTPUT);

  //Set default values of pins
    //LEDs
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    //Ult Sensors
    digitalWrite(ULT1_TRIG_PIN, LOW);
    digitalWrite(ULT2_TRIG_PIN, LOW);
    digitalWrite(ULT2_TRIG_PIN, LOW);
    //RS485
    digitalWrite(SERIAL_TRANSMIT_PIN, LOW);

  delayMicroseconds(20);
  serialDebugLN("Starting Arduino");
}


/////////////TEMPORARY FUNCTIONS////////////
bool tag() {
  bool sensor1, sensor2, sensor3;
  double distance;

  distance = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
  sensor1 = distance < 45;

  distance = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
  sensor2 = distance < 60;

  distance = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);
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

void runConveyorBeltAtSpeed(byte speed) {
   byte data[1] = { speed };
   serialSendData(NXT, data, 1, 3);
}
/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
	serialDebug(".");
}
