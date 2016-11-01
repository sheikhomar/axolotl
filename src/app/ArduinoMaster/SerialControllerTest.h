// SerialControllerTest.h

#ifndef _SERIALCONTROLLERTEST_h
#define _SERIALCONTROLLERTEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Functions
void serialSendTest();
void serialReceiveTest();
void serialArduinoNXTLoopTest();
void serialArduinoPICommTest();
void serialArduinoPICommTestHelperFunction(byte data[]);
void serialNoiseMaker();
void serialSendAllCharsTest();

#endif

