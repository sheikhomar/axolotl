// Defines.h

#ifndef _DEFINES_h
#define _DEFINES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Pins 
	//Ult Sensors
	#define ULT1_TRIG_PIN 2
	#define ULT1_ECHO_PIN 3
	#define ULT2_TRIG_PIN 4
	#define ULT2_ECHO_PIN 5
	#define ULT3_TRIG_PIN 6
	#define ULT3_ECHO_PIN 7
	//LEDs
	#define LED_GREEN 8
	#define LED_YELLOW 9
	#define LED_RED 10
	//Rs485
	#define RS485_RX 11
	#define RS485_TX 12
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
#define COMM_NXT_GET_COLOUR 5
#define COMM_NXT_ADJUST_BELT_SPEED 6

#define COMM_PI_ADVICEPACKAGE 'p'

#define COMM_ARDUINO_COLOUR_INFO 1


#define BIN_NOT_REQUESTED 222
#define BIN_REQUESTED 223
#define BIN_REQUESTED_AGAIN 224

#define COLOUR_RED 0
#define COLOUR_GREEN 1
#define COLOUR_BLUE 2
#define COLOUR_YELLOW 3
#define COLOUR_WHITE 6
#define COLOUR_BLACK 7
#define COLOUR_UNKNOWN 128
#define COLOUR_NOT_REQUESTED 222
#define COLOUR_REQUESTED 223

//Program variables
#define BAUD 57600
#define BAUD_DEBUG 9600
#define RS485_TIMEOUT_US 100
#define SERIAL_MAX_DATA_SIZE 12
#define SENSOR_BUFFER_SIZE 120
#define PACKAGE_BUFFER_SIZE 10
#define RS485_DATA_LENGTH_MAX 11
#define RS485_SERIAL_PRINT_BINARY 0
#define DEBUG 1

#define SENSOR_1 1
#define SENSOR_2 2
#define SENSOR_3 3

//Enums
typedef enum client {
	none = 'n',
	unknown = 'u',
	RaspberryPi = 'r',
	NXT = 'n',
	Debug_Client = ' ',
	Arduino = 'a'
} client;

typedef struct {
	unsigned int id = 0;
	unsigned int length = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	byte colour = COLOUR_NOT_REQUESTED;
	unsigned long middleTime = 0;
	byte bin = BIN_NOT_REQUESTED;
} Package;

typedef struct {
    int count = 0;
	unsigned long packageTimeoutMS = 0; //the time in ms when the current package should timeout
    Package items[PACKAGE_BUFFER_SIZE];
} PackageCollection;

typedef struct {
	double kalmanGain;
	double errorInEstimate; 
	double previousErrorInEstimate; 
	double errorInMeasurement; 
	double currentEstimate; 
	double previousEstimate;
} KalmanFilterInformation;



typedef struct {
	unsigned short sensor1 = 0;
	unsigned short sensor2 = 0;
	unsigned short sensor3 = 0;
	unsigned long time = 0;
} SensorData;

typedef struct {
	unsigned short sensorReadingBuffer[SENSOR_BUFFER_SIZE];
	unsigned short bufferCount = 0;
	unsigned long startTime = 0;
	unsigned long endTime = 0;
	byte falseCount = 0;
} SensorReading;

#endif

