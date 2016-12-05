// 
// 
// 

#include "Defines.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"
#include "PackageIdentification.h"

#include "PositionTest.h"

void determineTimeBetweenSensors(int numbOfRuns) {
	unsigned long iniTime = 0, timeColour = 0, travelTime = 0;
	const int HIGHEST_NUMB_OF_RUNS = 50;
	bool sensorRead = false;
	byte data2[1] = { 50 };
	unsigned long brickTimes[HIGHEST_NUMB_OF_RUNS];

	if (numbOfRuns <= HIGHEST_NUMB_OF_RUNS && numbOfRuns > 0) 
	{
		serialSendData(NXT, data2, 1, 6); //Set speed
		serialSendData(NXT, data2, 1, 3); //Start conveyor belt

		for (int i = 0; i < numbOfRuns; i++)
		{
			while (!sensorRead) { //Check when ult sensor register object
				sensorRead = readSensorsBest();
				delay(5); //earlier was ULT_MEASUREMENT_CYCLE_MS
			}

			sensorRead = false;

			iniTime = millis();
			readColourSensor(); //Reading colour sensor -- returning from func when a brick colour is read

			timeColour = millis();
			brickTimes[i] = timeColour - iniTime; //The time from when ult sensor register object untill colour sensor do
		}

		for (int j = 0; j < numbOfRuns; j++)
		{
			travelTime += brickTimes[j];
		}
		travelTime /= numbOfRuns;
		serialDebug("Time: ");
		serialDebug((String)travelTime);
		serialDebug("\n");
		delay(500);
		serialSendData(NXT, data2, 1, 4);
	}

	return;
}

void readColourSensor() {
	bool ready = false;
	byte dataS[1];
	byte dataRec[1];
	byte colour = -1;
	client sender = unknown;

	serialSendData(NXT, dataS, 1, 11);
	do {
		sender = serialReadData(dataRec, 1);
	} while (sender != Arduino);
	colour = dataRec[0];
	serialDebug("\n" + determineColour(colour));
}

bool readSensorsBest() { //Returning true when an object is detected by ult sensors
	bool sensor1, sensor2, sensor3;
	unsigned short dist1, dist2, dist3;

	/*dist1 = GetUltDistance(ULT_TOP_TRIG_PIN, ULT_TOP_ECHO_PIN, false);
	sensor1 = dist1 < ULT_TOP_TAG_DIST;

	dist2 = GetUltDistance(ULT_RIGHT_TRIG_PIN, ULT_RIGHT_ECHO_PIN, false);
	sensor2 = dist2 < ult2_TagDist;*/

	dist3 = GetUltDistance(ULT_LEFT_TRIG_PIN, ULT_LEFT_ECHO_PIN, false);
	sensor3 = dist3 < ULT_LEFT_TAG_DIST;

	return sensor3;
}

String determineColour(byte input) {
	switch (input) {
	case 0: return "Red";
	case 1: return "Green";
	case 2: return "Blue";
	case 3: return "Yellow";
	default: return "Irrelevant";
	}
}

