// UltrasoundSensor.h

#ifndef _ULTRASOUNDSENSOR_h
#define _ULTRASOUNDSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Functions
void ultSetup(long ultA0, long ultA1, long ultB0, long ultB1, long ultC0, long ultC1);
unsigned short GetUltDistance(int trigPin, int echoPin, bool wait);


#endif

