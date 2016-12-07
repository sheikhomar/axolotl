// PackageIdentification.h

#ifndef _PACKAGEIDENTIFICATION_h
#define _PACKAGEIDENTIFICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Defines
#define ULT_TOP_TAG_DIST 3800					// TODO: Calibrate this
#define ULT_RIGHT_TAG_DIST 5500					// TODO: Calibrate this
#define ULT_LEFT_TAG_DIST 4500 					// TODO: Calibrate this

#define NOT_DETECTED_THRESHOLD 4
#define SPEED_CONVEYOR 180				// TODO: Calibrate this

#define LENGTH_BETWEEN_SENSORS 11300			// TODO: Calibrate this  -- Old: 11950
#define HEIGHT_BETWEEN_SENSOR_AND_BELT 4950 // TODO: Calibrate this


#define TOP_SENSOR_CHECK_DISTANCE 50 // TODO: Calibrate this
#define LEFT_SENSOR_CHECK_DISTANCE 50 // TODO: Calibrate this
#define RIGHT_SENSOR_CHECK_DISTANCE 50 // TODO: Calibrate this

#define TRANSPORT_TIME 500 // transport time through the sensors in millis

#define CORRECT_AMOUNT_THRESHOLD 3

//Functions
short makeReading(int whichSensor);
bool checkReading(int whichSensor, int dist);
void addReading(SensorReading *reading, unsigned short dist);
void checkAndIncrement(SensorReading *reading, bool bVal);

void handleSensorReadings(Package *package, SensorReading *sensor1, SensorReading *sensor2, SensorReading *sensor3);
unsigned long findLength(SensorReading *sensor);
unsigned long normalizeSensorData(SensorReading *sensor);
unsigned long findMiddleTime(SensorReading *sensor);
void printPackageSize(Package *package);

void initPackageIdentification(PackageIdentificationState *state);
void initSensorBuffer(SensorBuffer *buffer);
void runIdentification(PackageIdentificationState *state, PackageCollection *packages);
void createSensorResult(bool tag, SensorBuffer *sensorBuffer, unsigned short sensorCheckDistance, String s);
unsigned short calculateSensorResult(ReadingCollection *collection, long checkDistance);
bool performReading(PackageIdentificationState *state, SensorBuffer *buffer, int whichSensor);
void addItemToCollection(ReadingCollection *collection, double estimate);
void queueResult(SensorBuffer *sensorBuffer, SensorResultQueue *queue);
void setPackageInfo(Package *package, SensorResult *leftResult, SensorResult *topResult, SensorResult *rightResult);
void checkForFailedSensor(unsigned long endtime, PackageIdentificationState *state);
SensorResult dequeue(SensorResultQueue *queue);
unsigned long calcLength(SensorResult *result);
unsigned long findMedian(unsigned long left, unsigned long top, unsigned long right);
void sort(unsigned long buffer[], int size);

#endif

