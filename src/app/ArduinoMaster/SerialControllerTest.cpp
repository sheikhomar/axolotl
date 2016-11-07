// 
// 
// 

#include "Defines.h"
#include "SerialController.h"
#include "LEDController.h"

#include "SerialControllerTest.h"

/***************************
serialSendTest

Broadcasts the alphabet out on the RS485 network
***************************/
void serialSendTest()
{
	int i;
	const int sizeOfData = 26;
	byte myArr[sizeOfData];

	for (i = 0; i<sizeOfData; i++) {
		myArr[i] = 'a' + i;
	}

	serialSendData(NXT, myArr, sizeOfData, 0);
}

/***************************
serialReceiveTest


***************************/
void serialReceiveTest() {
	byte byteReceived[] = { 0 };
	client sender = none;

	if (serialCheck())
	{
		led(LED_RED, true);
		sender = serialReadData(byteReceived, 1);
		if (sender != none) {
			serialDebug(String(byteReceived[0]));
		}
		delay(10);
		led(LED_RED, false);
	}
}

/***************************
serialArduinoNXTLoopTest

Combines the sending and receiving of data together with the NXT.
A byte is incremented from 'a' to 'z', where both the Arduino and NXT increments the value by one.
***************************/
void serialArduinoNXTLoopTest() {
	byte data[] = { 'a' };
	client sender = unknown;

	while (data[0] <= 'y')
	{
		serialSendData(NXT, data, 1, 10);

		do {
			sender = serialReadData(data, 1);
		} while (sender != Arduino);
		String masterString = "|Data=";
		masterString.concat(data[0]);
		masterString.concat("|");

		data[0] += 1;
	}
}

/***************************
serialArduinoPICommTest

Combines the sending and receiving of data together with the PI.
Sends five packages to the PI and blinks the builtin LED in acc with which motor to push.
***************************/
void serialArduinoPICommTest() {
	byte packages[5][4] = {
		{ 1,1,1,COLOUR_RED },
		{ 2,2,2,COLOUR_BLUE },
		{ 4,4,4,COLOUR_YELLOW },
		{ 8,8,8,COLOUR_YELLOW },
		{ 1,2,3,COLOUR_GREEN }
	};
	byte i;

	for (i = 0; i < 5; i++)
	{
		serialArduinoPICommTestHelperFunction(packages[i]);
	}
}

void serialArduinoPICommTestHelperFunction(byte data[]) {
	client sender = unknown;
	byte received[] = { 0 };

	serialSendData(RaspberryPi, data, 4, COMM_PI_ADVICEPACKAGE);

	do {
		sender = serialReadData(received, 1);
	} while (sender != RaspberryPi);


	while (received[0] > 0) {
		led(LED_BUILTIN, true);
		delay(500);
		received[0] -= 1;
	}
}


/***************************
serialSendAllCharsTest

Sends all possible ASCII chars on the debug network
***************************/
void serialSendAllCharsTest() {
	byte data[] = { '_', 0, '_' };
	int i;

	for (i = 0; i < 256; i++)
	{
		data[1] = i;

		if (RS485_SERIAL_PRINT_BINARY) {
			serialSendData(Debug_Client, data, 3, 10);
			Serial.print(data[1]);
		}
		else {
			serialSendData(Debug_Client, data, 3, 10);
			Serial.println("");
		}

	}
}