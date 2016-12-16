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
	#define ULT_TOP_TRIG_PIN 2
	#define ULT_TOP_ECHO_PIN 3
	#define ULT_RIGHT_TRIG_PIN 4
	#define ULT_RIGHT_ECHO_PIN 5
	#define ULT_LEFT_TRIG_PIN 6
	#define ULT_LEFT_ECHO_PIN 7
	//LEDs
	#define LED_RED 8
	#define LED_YELLOW 9
	#define LED_GREEN 10
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
#define COLOUR_UNKNOWN 127
#define COLOUR_NOT_REQUESTED 222
#define COLOUR_REQUESTED 223

//Program variables
#define BAUD 57600
#define BAUD_DEBUG 9600
#define RS485_TIMEOUT_US 100
#define SERIAL_MAX_DATA_SIZE 12
#define PACKAGE_BUFFER_SIZE 10
#define SENSOR_READINGS_SIZE 30 
#define SLIDING_WINDOW_K 10
#define RS485_DATA_LENGTH_MAX 11
#define RS485_SERIAL_PRINT_BINARY 0
#define DEBUG 1
#define SENSOR_RESULT_SIZE 5
#define PI_REQUEST_TIMEOUT_MS 500
#define NXT_REQUEST_TIMEOUT_MS 500

#define ULT_TOP_SENSOR 1
#define ULT_RIGHT_SENSOR 2
#define ULT_LEFT_SENSOR 3

#define KALMAN_OR_SLIDING 1           // 1 - to use kalman    0 - to use sliding


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
	unsigned long packageTimeoutMS = PI_REQUEST_TIMEOUT_MS; //the time in ms when the current package should timeout
	unsigned long colourTimeoutMS = NXT_REQUEST_TIMEOUT_MS; //the time in ms when the current colour request should timeout
    Package items[PACKAGE_BUFFER_SIZE];
} PackageCollection;

typedef struct {
    double kalmanGain;
    double errorInEstimate;
    double sensorNoise;
    double currentEstimate;
    double processNoise;
} KalmanFilterInformation;

typedef struct {
	double measurements[SLIDING_WINDOW_K];
	short count;
	double sumOfMeasurements;
	double currentValue;
} SlidingWindowInformation;

typedef struct {
    unsigned short readings[SENSOR_READINGS_SIZE];
    int count = 0;
} ReadingCollection;

typedef struct {
    ReadingCollection data;
    unsigned short result;
    bool isReady;
    unsigned long startTime;
    unsigned long endTime;
} SensorBuffer;

typedef struct {
    unsigned short result;
    unsigned long startTime;
    unsigned long endTime;
} SensorResult;

typedef struct {
    SensorResult data[SENSOR_RESULT_SIZE];
    int count;
} SensorResultQueue;

typedef struct {
	#if KALMAN_OR_SLIDING
		KalmanFilterInformation kalmanTopSensor;
		KalmanFilterInformation kalmanRightSensor;
		KalmanFilterInformation kalmanLeftSensor;
	#else
		SlidingWindowInformation slidingTopSensor;
		SlidingWindowInformation slidingRightSensor;
		SlidingWindowInformation slidingLeftSensor;
	#endif
    
	SensorBuffer topSensorBuffer;
    SensorBuffer rightSensorBuffer;
    SensorBuffer leftSensorBuffer;

    SensorResultQueue topSensorResultQueue;
    SensorResultQueue rightSensorResultQueue;
    SensorResultQueue leftSensorResultQueue;
} PackageIdentificationState;

#endif

