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
#define ult2_TagDist 4000
#define ult3_TagDist 3500 // TODO: This must be adjusted.

#define ARRAY_SIZE 20
#define SPEED_CONVEYOR 140


//Functions
bool readSensors(SensorData *sensorData);
void handleSensorData(Package *package, SensorData buffer[], int bufferStartIndex, int bufferCount);
unsigned long findAverage(SensorData buffer[], int startIndex, int bufferSize, byte whichSensor);
int findMode(SensorData buffer[], int bufferStartIndex, int bufferCount, byte sensor);



#endif

