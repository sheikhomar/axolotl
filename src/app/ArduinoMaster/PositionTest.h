// PositionTest.h

#ifndef _POSITIONTEST_h
#define _POSITIONTEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Functions
void determineTimeBetweenSensors();
void readColourSensor();
bool readSensorsBest();
String determineColour(byte input);


#endif

