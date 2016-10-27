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
    byte bin;
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
  #define LEDB_PIN LED_BUILTIN

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
  #define ULT_MEASUREMENT_CYCLE_MS 60
  #define ULT_TRIG_PULSE_US 10

#define COMM_NXT_PUSH_ARM1 0
#define COMM_NXT_PUSH_ARM2 1
#define COMM_NXT_PUSH_BOTH_ARMS 2
#define COMM_NXT_START_BELT 3
#define COMM_NXT_STOP_BELT 4
#define COMM_NXT_GET_COLOUR 11
#define COMM_NXT_ADJUST_BELT_SPEED 6


//Program variables
  #define BAUD 57600
  #define BAUD_DEBUG 9600
  #define SERIAL_MAX_DATA_SIZE 12
  #define SENSOR_BUFFER_SIZE 100
  #define PACKAGE_BUFFER_SIZE 10
  #define RS485_DATA_LENGTH_MAX 11

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

/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
	serialArduinoNXTLoopTest();
}
