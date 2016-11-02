/*
* Master Program
*
*/

//External libs
#include <SoftwareSerial.h>

//Internal libs
#include "Defines.h"
#include "UltrasoundSensorTest.h"
#include "SerialControllerTest.h"
#include "PositionTest.h"
#include "Utilities.h"
#include "UltrasoundSensor.h"
#include "SerialController.h"
#include "Scheduler.h"
#include "ImprovedScheduler.h"
#include "ObjectIdentification.h"
#include "LEDController.h"

void setup() {
	//Initiate Serial Communication
	serialSetup(BAUD, BAUD_DEBUG);
	ledSetup(LED_RED,LED_YELLOW,LED_GREEN,LED_BUILTIN);

	//Initiate Pins
	//Ult Sensors
	pinMode(ULT1_TRIG_PIN, OUTPUT);
	pinMode(ULT1_ECHO_PIN, INPUT);
	pinMode(ULT2_TRIG_PIN, OUTPUT);
	pinMode(ULT2_ECHO_PIN, INPUT);
	pinMode(ULT3_TRIG_PIN, OUTPUT);
	pinMode(ULT3_ECHO_PIN, INPUT);
	//RS485
	pinMode(SERIAL_TRANSMIT_PIN, OUTPUT);

	//Set default values of pins
	//Ult Sensors
	digitalWrite(ULT1_TRIG_PIN, LOW);
	digitalWrite(ULT2_TRIG_PIN, LOW);
	digitalWrite(ULT2_TRIG_PIN, LOW);
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
