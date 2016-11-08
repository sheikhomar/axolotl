// ObjectIdentification.h

#ifndef _OBJECTIDENTIFICATION_h
#define _OBJECTIDENTIFICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Defines
#define ult1_TagDist 3800
#define ult2_TagDist 5800
#define ult3_TagDist 3600 // TODO: This must be adjusted.

#define ARRAY_SIZE 20
#define SPEED_CONVEYOR 140

#define LENGTH_BETWEEN_SENSORS 11950
#define HEIGHT_BETWEEN_SENSOR_AND_BELT 5200

//Functions
bool readSensors(SensorData *sensorData);
bool readSensor(SensorReading sensorBuffer[], int *bufferCount, int sensor);
void handleSensorData(Package *package, SensorData buffer[], int bufferStartIndex, int bufferCount);
void handleSensorReadings(Package *package, SensorReading sensorBuffer1[], int sensorBuffer1Count,
	SensorReading sensorBuffer2[], int sensorBuffer2Count,
	SensorReading sensorBuffer3[], int sensorBuffer3Count);
unsigned long findAverage(SensorData buffer[], int startIndex, int bufferSize, byte whichSensor);
int findMode(SensorData buffer[], int bufferStartIndex, int bufferCount, byte sensor);
unsigned long findLength(SensorReading buffer[], int bufferCount);
unsigned long normalizeSensorData(SensorReading buffer[], int bufferCount);
unsigned long findMiddleTime(SensorReading buffer[], int bufferCount);


#endif

