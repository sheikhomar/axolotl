/*
* Master Program
*
*/

//External libs
#include <SoftwareSerial.h>

//Internal libs
#include "Defines.h"
#include "Utilities.h"
#include "UltrasoundSensor.h"
#include "SerialController.h"
#include "Scheduler.h"
#include "ImprovedScheduler.h"
#include "ObjectIdentification.h"
#include "LEDController.h"
#include "LEDControllerTest.h"

void setup() {
	//Initiate Serial Communication
	serialSetup(BAUD, BAUD_DEBUG);
	ledSetup(LED_RED,LED_YELLOW,LED_GREEN,LED_BUILTIN);
	ultSetup(ULT1_TRIG_PIN, ULT1_ECHO_PIN, ULT2_TRIG_PIN, ULT2_ECHO_PIN, ULT3_TRIG_PIN, ULT3_ECHO_PIN);

	//Initiate Pins
	//RS485
	pinMode(SERIAL_TRANSMIT_PIN, OUTPUT);

	//Set default values of pins
	//RS485
	digitalWrite(SERIAL_TRANSMIT_PIN, LOW);

	delayMicroseconds(20);
	serialDebugLN("Starting Arduino");
}

/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
	runConveyorBeltAtSpeed(50);
	runScheduler();
}
