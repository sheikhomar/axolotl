// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
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

	distance1 = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
	//delay?
	distance2 = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
	distance3 = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);

	//Create debug string
	String masterString;
	masterString.concat(String(distance1));
	masterString.concat("\t");
	masterString.concat(String(distance2));
	masterString.concat("\t");
	masterString.concat(String(distance3));
	masterString.concat("\t");

	masterString.concat(String(distance1 < ult1_TagDist));
	masterString.concat("  ");
	masterString.concat(String(distance2 < ult2_TagDist));
	masterString.concat("  ");
	masterString.concat(String(distance3 < ult3_TagDist));
	serialDebugLN(masterString);

	delay(ULT_MEASUREMENT_CYCLE_MS);
}
