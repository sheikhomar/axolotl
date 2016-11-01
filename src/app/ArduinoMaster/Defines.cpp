// 
// 
// 

#include "Defines.h"

//Enums
typedef enum client {
	none = 'n',
	unknown = 'u',
	RaspberryPi = 'r',
	NXT = 'n',
	DEBUG = ' ',
	Arduino = 'a'
} client;

typedef struct {
	unsigned int length;
	unsigned int width;
	unsigned int height;
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
#define COMM_NXT_GET_COLOUR 5
#define COMM_NXT_ADJUST_BELT_SPEED 6

#define COMM_PI_ADVICEPACKAGE 'p'


//Program variables
#define BAUD 57600
#define BAUD_DEBUG 9600
#define RS485_TIMEOUT_US 100
#define SERIAL_MAX_DATA_SIZE 12
#define SENSOR_BUFFER_SIZE 100
#define PACKAGE_BUFFER_SIZE 10
#define RS485_DATA_LENGTH_MAX 11
