// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "LEDController.h"

#include "UltrasoundSensor.h"

/*
* Test doc of Ult sensor
*
* Manual link: http://www.micropik.com/PDF/HCSR04.pdf
*/


/***************************
GetDistance

Get the distance from a specified ultraSoundSensor via its triggerPin and echoPin.
The delay can be used to avoid polling issues should the same sensor be called multiple times in a row.
Returns the distance in mm * 100
***************************/
unsigned short GetUltDistance(int trigPin, int echoPin, bool wait) {
	unsigned long duration, distance;

	if (wait)
		delay(ULT_MEASUREMENT_CYCLE_MS);

	//TriggerPulse
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(ULT_TRIG_PULSE_US);
	digitalWrite(trigPin, LOW);

	//Receive echo signal
	duration = pulseIn(echoPin, HIGH);

	return (unsigned short)(duration*ULT_MM_PER_US * 100);
}



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
