// ImprovedScheduler.h

#ifndef _IMPROVEDSCHEDULER_h
#define _IMPROVEDSCHEDULER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Defines.h"

//Defines
#define FROM_ULT_TO_COLOUR_SENSOR_MS 2098
#define FROM_ULT_TO_ARM1_MS 2564
#define FROM_ULT_TO_ARM2_MS 3497

//Functions
void resetImprovedPackage(Package *package);
void resetImprovedPackages(Package packages[]);
void resetSensorReading(SensorReading *reading);
void runImprovedScheduler();
void checkBufferCount(short buffCount);
void cleanBuffer(SensorReading *reading, short sensor);

#endif

