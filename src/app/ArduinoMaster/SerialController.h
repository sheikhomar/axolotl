// SerialController.h

#ifndef _SERIALCONTROLLER_h
#define _SERIALCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Functions
client serialCheck();
void serialSendData(client receiver, byte data[], byte sizeOfData, byte reciverFunction);
void serialSendData(client receiver, byte reciverFunction);
client serialReadData(byte data[], int data_length);
bool serialBuffTimeout(byte numberOfRequiredElements);
void serialDebug(String message);
void serialDebugLN(String message);

void serialSendTest();
void serialReceiveTest();
void serialArduinoNXTLoopTest();
void serialArduinoPICommTest();
void serialArduinoPICommTestHelperFunction(byte data[]);
void serialNoiseMaker();




#endif

