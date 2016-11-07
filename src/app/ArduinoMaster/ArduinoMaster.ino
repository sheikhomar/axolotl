/*
* Master Program
*
*/

//External libs
#include <SoftwareSerial.h>

//Internal libs
#include "Defines.h"
#include "Scheduler.h"
#include "ImprovedScheduler.h"
#include "LEDController.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "LEDControllerTest.h"
#include "PositionTest.h"
#include "SerialControllerTest.h"
#include "UltrasoundSensorTest.h"

void setup() {
	//Initiate Serial Communication
	serialSetup(BAUD, BAUD_DEBUG);
	ledSetup(LED_RED,LED_YELLOW,LED_GREEN,LED_BUILTIN);
	ultSetup(ULT1_TRIG_PIN, ULT1_ECHO_PIN, ULT2_TRIG_PIN, ULT2_ECHO_PIN, ULT3_TRIG_PIN, ULT3_ECHO_PIN);

	delayMicroseconds(20);
	serialDebugLN("Starting Arduino");
}

/***************************
loop

Main control loop of the Arduino.
***************************/
void loop() {
	//runScheduler();
	runImprovedScheduler();
}