// ObjectIdentification.h

#ifndef _OBJECTIDENTIFICATION_h
#define _OBJECTIDENTIFICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Defines
#define ult1_TagDist 3800					// TODO: Calibrate this
#define ult2_TagDist 5800					// TODO: Calibrate this
#define ult3_TagDist 3600					// TODO: Calibrate this

#define ARRAY_SIZE 20
#define NOT_DETECTED_THRESHOLD 5
#define SPEED_CONVEYOR 140					// TODO: Calibrate this

#define LENGTH_BETWEEN_SENSORS 11950			// TODO: Calibrate this
#define HEIGHT_BETWEEN_SENSOR_AND_BELT 5200 // TODO: Calibrate this

//Functions
bool readSensor(SensorReading *reading, int sensor);
short makeReading(int whichSensor);
bool checkReading(int whichSensor, int dist);
void addReading(SensorReading *reading, unsigned short dist);
void checkAndIncrement(SensorReading *reading, bool bVal);

void handleSensorReadings(Package *package, SensorReading *sensor1, SensorReading *sensor2, SensorReading *sensor3);
unsigned long findLength(SensorReading *sensor);
unsigned long normalizeSensorData(SensorReading *sensor);
unsigned long findMiddleTime(SensorReading *sensor);
void printPackageSize(Package *package);


#endif

