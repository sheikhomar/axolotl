// Utilities.h

#ifndef _UTILITIES_h
#define _UTILITIES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Functions
void die(String abortMessage);
void runConveyorBeltAtSpeed(byte speed);
void monitorRS485();
#endif

