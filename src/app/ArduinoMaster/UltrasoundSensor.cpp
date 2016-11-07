// 
// 
// Manual link for the sensor: http://www.micropik.com/PDF/HCSR04.pdf

#include "Defines.h"

#include "UltrasoundSensor.h"

/***************************
serialSetup

Initialize led pins
***************************/
void ultSetup(long ultA0, long ultA1, long ultB0, long ultB1, long ultC0, long ultC1) {
	pinMode(ultA0, OUTPUT);
	pinMode(ultA1, INPUT);
	pinMode(ultB0, OUTPUT);
	pinMode(ultB1, INPUT);
	pinMode(ultC0, OUTPUT);
	pinMode(ultC1, INPUT);

	digitalWrite(ultA0, LOW);
	digitalWrite(ultB0, LOW);
	digitalWrite(ultC0, LOW);
}


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