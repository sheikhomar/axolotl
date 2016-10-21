void determineTimeBetweenSensors() {
	unsigned long iniTime = 0, timeColour = 0;
	bool sensorRead = false;
	double travelTime = 0;
	byte data[] = { 'T','r','a','v','e','l','t','i','m','e',':',' ' };
	byte data2[32];

	while (!sensorRead) {
		sensorRead = tag();
	}

	iniTime = millis();
	sensorRead = false;

	while (!sensorRead) {
		sensorRead = colourSensorReady();
	}

	timeColour = millis();
	timeColour = timeColour - iniTime;
	travelTime = (double)timeColour;
	//SerialSendData(Debug, data);
	//SerialSendData(travelTime);
	return;
}

bool colourSensorReady() {
	bool ready = false;
	byte dataS[] = { 50 };
	int colour = -1;
	serialSendData(NXT, dataS, 0, 5);

	while (colour != 0 || colour != 1 || colour != 2 || colour != 3 || colour != 6 || colour != 7) {
		//colour = serialReadData();
	}
	return true;
}

