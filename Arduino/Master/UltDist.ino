/*
* Test doc of Ult sensor
*
* Manual link: http://www.micropik.com/PDF/HCSR04.pdf
*/


/***************************
GetDistance

Get the distance from a specified ultraSoundSensor via its triggerPin and echoPin.
The delay can be used to avoid polling issues should the same sensor be called multiple times in a row.
***************************/
double GetUltDistance(int trigPin, int echoPin, bool wait) {
	double duration, distance;

	if (wait)
		delay(ultMeasurementCycle);

	//TriggerPulse
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(ultTrigPulse);
	digitalWrite(trigPin, LOW);

	//Receive echo signal
	duration = pulseIn(echoPin, HIGH);

	return duration*ultMmPerUS;
}



/***************************
TestUltSensors

Prints the measured distance of all UltSensors and turns on their LEDs in acc.
to its idle distance.
***************************/
void TestUltSensors() {
	double distance;

	Serial.print("Sensor 1 |");
	distance = GetUltDistance(ult1_TrigPin, ult1_echoPin, false);
	PrintUltSensorData(distance, distance < 45);

	Serial.print("Sensor 2 |");
	distance = GetUltDistance(ult2_TrigPin, ult2_echoPin, false);
	PrintUltSensorData(distance, distance < 60);

	Serial.print("Sensor 3 |");
	distance = GetUltDistance(ult3_TrigPin, ult3_echoPin, false);
	PrintUltSensorData(distance, distance < 33);

	delay(ultMeasurementCycle);
}

/***************************
PrintDistance

Prints the distance on the serial port and turns on/off the led based on tag
***************************/
void PrintUltSensorData(double distance, bool tag) {
	Serial.print(distance);
	Serial.print(" mm ");

	if (tag) {
		Serial.println("| Tag");
	}
	else {
		Serial.println("");
	}
}