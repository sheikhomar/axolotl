// UltrasoundSensor.h

#ifndef _ULTRASOUNDSENSOR_h
#define _ULTRASOUNDSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Functions
unsigned short GetUltDistance(int trigPin, int echoPin, bool wait);
void TestUltSensors();


#endif

