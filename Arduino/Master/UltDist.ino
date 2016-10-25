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
		delay(ULT_MEASUREMENT_CYCLE);

	//TriggerPulse
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(ULT_TRIG_PULSE);
	digitalWrite(trigPin, LOW);

	//Receive echo signal
	duration = pulseIn(echoPin, HIGH);

	return (unsigned short)(duration*ULT_MM_PER_US*100);
}



/***************************
TestUltSensors

Prints the measured distance of all UltSensors and turns on their LEDs in acc.
to its idle distance.
***************************/
void TestUltSensors() {
	unsigned short distance;

	serialDebug("Sensor 1 |");
	distance = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
	PrintUltSensorData(distance, distance < ult1_TagDist);

	serialDebug("Sensor 2 |");
	distance = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
	PrintUltSensorData(distance, distance < ult2_TagDist);

	serialDebug("Sensor 3 |");
	distance = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);
	PrintUltSensorData(distance, distance < ult3_TagDist);

	delay(ULT_MEASUREMENT_CYCLE);
}

/***************************
PrintDistance

Prints the distance on the serial port and turns on/off the led based on tag
***************************/
void PrintUltSensorData(unsigned short distance, bool tag) {
	serialDebug(String(distance));
	serialDebug(" mm ");

	if (tag) {
		serialDebugLN("| Tag");
	}
	else {
		serialDebugLN("");
	}
}
