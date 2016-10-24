void determineTimeBetweenSensors() {
	unsigned long iniTime = 0, timeColour = 0;
	bool sensorRead = false;
	double travelTime = 0;
	byte data[] = { 'T','r','a','v','e','l','t','i','m','e',':',' ' };
	byte data2[32];

//serialDebug("Her ");
	while (!sensorRead) {
		sensorRead = readSensors();
   //switch(sensorRead){
   // case false: serialDebug("false"); break;
   // case true: serialDebug("true"); break;
   // default: break;
   //}
   delay(ultMeasurementCycle);
	}
serialDebug("Her1");
	iniTime = millis();
	sensorRead = false;
//serialDebug("Her2");
	while (!sensorRead) {
		sensorRead = colourSensorReady();
	}
serialDebug("Her3");
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
	byte colour = -1;

	while (colour != 0 || colour != 1 || colour != 2 || colour != 3 || colour != 6 || colour != 7) {
    serialSendData(NXT, dataS, 0, 5);
		serialReadData(dataRec, 1);
    colour = dataRec[0] +52;
    serialDebug("" + colour);
    delay(500);
	}
	return true;
}

