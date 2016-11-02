// LEDControllerTest.h

#ifndef _LEDCONTROLLERTEST_h
#define _LEDCONTROLLERTEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Functions
void ledBlinkTest();
void ledBlinkTestHelperFunction(long ledPin, long time, String message);

#endif

