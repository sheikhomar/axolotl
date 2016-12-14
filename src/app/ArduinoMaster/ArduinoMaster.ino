/*
* Master Program
*
*/

//External libs
#include "KalmanFilterTest.h"
#include "SchedulerTest.h"
#include "FilterTest.h"
#include "SlidingWindowFilter.h"
#include "KalmanFilter.h"
#include <SoftwareSerial.h>

//Internal libs
#include "Defines.h"
#include "Scheduler.h"
#include "LEDController.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "LEDControllerTest.h"
#include "PositionTest.h"
#include "SerialControllerTest.h"
#include "UltrasoundSensorTest.h"
#include "Utilities.h"

void setup() {
	//Initiate Serial Communication
	serialSetup(BAUD, BAUD_DEBUG);
	ledSetup(LED_RED,LED_YELLOW,LED_GREEN);
	ultSetup(ULT_TOP_TRIG_PIN, ULT_TOP_ECHO_PIN, ULT_RIGHT_TRIG_PIN, ULT_RIGHT_ECHO_PIN, ULT_LEFT_TRIG_PIN, ULT_LEFT_ECHO_PIN);

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