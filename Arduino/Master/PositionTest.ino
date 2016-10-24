void determineTimeBetweenSensors() {
	unsigned long iniTime = 0, timeColour = 0;
	bool sensorRead = false;
	double travelTime = 0;
	byte data[] = { 'T','r','a','v','e','l','t','i','m','e',':',' ' };
	byte data2[32];

serialDebug("Her ");
	while (!sensorRead) {
		sensorRead = tag();
	}
serialDebug("Her1");
	iniTime = millis();
	sensorRead = false;

	while (!sensorRead) {
		sensorRead = colourSensorReady();
	}

	timeColour = millis();
	timeColour = timeColour - iniTime;
	travelTime = (double)timeColour;
	serialDebug("Traveltime: ");
	serialDebug((String)timeColour);
	return;
}

bool colourSensorReady() {
	bool ready = false;
	byte dataS[] = {5};
  byte dataRec[1];
	int colour = -1;
	serialSendData(NXT, dataS, 0, 5);

	while (colour != 0 || colour != 1 || colour != 2 || colour != 3 || colour != 6 || colour != 7) {
		serialReadData(dataRec, 1);
    colour = dataRec[0];
	}
	return true;
}

