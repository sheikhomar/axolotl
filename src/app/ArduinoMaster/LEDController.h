// LEDController.h

#ifndef _LEDCONTROLLER_h
#define _LEDCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Functions
void ledSetup(long ledA, long ledB, long ledC);
void led(int LED, bool TurnOn);


#endif

