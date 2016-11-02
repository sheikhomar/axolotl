// 
// 
// Manual link for the sensor: http://www.micropik.com/PDF/HCSR04.pdf

#include "Defines.h"

#include "UltrasoundSensor.h"

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