#define ult1_TagDist 45
#define ult2_TagDist 60
#define ult3_TagDist 33
#define ARRAY_SIZE 20
#define SPEED 140




//read all 3 sensor data - save data somewhere - return boolean to tell if an object was deteced
bool readSensors(int index) {
	bool sensor1, sensor2, sensor3;
	unsigned short dist1, dist2, dist3;

	dist1 = GetUltDistance(ult1_TrigPin, ult1_echoPin, false);
	sensor1 = dist1 < ult1_TagDist;

	dist2 = GetUltDistance(ult2_TrigPin, ult2_echoPin, false);
	sensor2 = dist2 < ult2_TagDist;

	dist3 = GetUltDistance(ult3_TrigPin, ult3_echoPin, false);
	sensor3 = dist3 < ult3_TagDist;

	if (sensor1 && sensor2) {
		sensorBuffer[index].sensor1 = dist1;
		sensorBuffer[index].sensor2 = dist2;
		sensorBuffer[index].sensor3 = dist3;
		sensorBuffer[index].time = millis();

		return true;
	}
	else {
		heigthBetweenSensorAndBelt = dist1;
		lengthBetweenSensors = (dist2 + dist3) / 2;
		return false;
	}
}



//based on the collected data (so far) create the object
void handleSensorData(short dimensionSize, short packageIndex, short lastBufferItemIndex) {
	unsigned short sensor1, sensor2, sensor3;
	unsigned short packageTime;

	sensor1 = findMode(dimensionSize, 1);
	sensor2 = findMode(dimensionSize, 2);
	sensor3 = findMode(dimensionSize, 3);

	packages[packageIndex].height = heigthBetweenSensorAndBelt - sensor1; 
	packages[packageIndex].width = lengthBetweenSensors - sensor2 - sensor3;
	packageTime = (sensorBuffer[lastBufferItemIndex].time - sensorBuffer[0].time);
	packages[packageIndex].length = packageTime * SPEED;
}

int findMode(short dimensionSize, short sensor) {
	int countArray[ARRAY_SIZE][2];
	int currentLength = 0;
	unsigned short sensorData = 0;
	bool wasDetected = false;
	
	for (int i = 0; i <= dimensionSize; i++) {
		if (sensor == 1)
			sensorData = sensorBuffer[i].sensor1;
		else if (sensor == 2)
			sensorData = sensorBuffer[i].sensor2;
		else if (sensor == 3)
			sensorData = sensorBuffer[i].sensor3;

		for (int j = 0; j <= currentLength; j++) {
			if (sensorData == countArray[j][0]) {
				countArray[j][1]++;
				wasDetected = true;
			}
		}
		if (!wasDetected) {
			countArray[currentLength][0] = sensorData;
			countArray[currentLength][1] = 1;
		}
	}

	unsigned short mostCommonValue = 0;
	short largestCount = 0;

	for (int i = 0; i <= ARRAY_SIZE; i++) {
		if (countArray[i][1] > largestCount) {
			largestCount = countArray[i][1];
			mostCommonValue = countArray[i][0];
		}
		else if (countArray[i][1] == largestCount) {
			mostCommonValue = (mostCommonValue + countArray[i][0])/2;
		}
	}

	return mostCommonValue;
}