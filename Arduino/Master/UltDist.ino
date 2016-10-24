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
		delay(ultMeasurementCycle);

	//TriggerPulse
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(ultTrigPulse);
	digitalWrite(trigPin, LOW);

	//Receive echo signal
	duration = pulseIn(echoPin, HIGH);

	return (unsigned short)(duration*ultMmPerUS*100);
}



/***************************
TestUltSensors

Prints the measured distance of all UltSensors and turns on their LEDs in acc.
to its idle distance.
***************************/
void TestUltSensors() {
	unsigned short distance;

	serialDebug("Sensor 1 |");
	distance = GetUltDistance(ult1_TrigPin, ult1_echoPin, false);
	PrintUltSensorData(distance, distance < ult1_TagDist);

	serialDebug("Sensor 2 |");
	distance = GetUltDistance(ult2_TrigPin, ult2_echoPin, false);
	PrintUltSensorData(distance, distance < ult2_TagDist);

	serialDebug("Sensor 3 |");
	distance = GetUltDistance(ult3_TrigPin, ult3_echoPin, false);
	PrintUltSensorData(distance, distance < ult3_TagDist);

	delay(ultMeasurementCycle);
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
