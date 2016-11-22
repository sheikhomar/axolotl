// 
// 
// 

#include "Defines.h"
#include "SerialController.h"
#include "LEDController.h"
#include "UltrasoundSensor.h"
#include "ObjectIdentification.h"

#include "UltrasoundSensorTest.h"


/***************************
TestUltSensors

Prints the measured distance of all UltSensors and whether they will TAG.
***************************/
void TestUltSensors() {
	unsigned short distance1, distance2, distance3;

	distance2 = GetUltDistance(ULT_RIGHT_TRIG_PIN, ULT_RIGHT_ECHO_PIN, true);
	distance1 = GetUltDistance(ULT_TOP_TRIG_PIN, ULT_TOP_ECHO_PIN, true);
	distance3 = GetUltDistance(ULT_LEFT_TRIG_PIN, ULT_LEFT_ECHO_PIN, true);

	//Create debug string
	String masterString;
	masterString.concat(String(distance1));
	masterString.concat("\t");
	masterString.concat(String(distance2));
	masterString.concat("\t");
	masterString.concat(String(distance3));
	masterString.concat("\t");

	masterString.concat(String(distance1 < ULT_TOP_TAG_DIST));
	masterString.concat("  ");
	masterString.concat(String(distance2 < ULT_RIGHT_TAG_DIST));
	masterString.concat("  ");
	masterString.concat(String(distance3 < ULT_LEFT_TAG_DIST));
	serialDebugLN(masterString);

	delay(ULT_MEASUREMENT_CYCLE_MS);
}
