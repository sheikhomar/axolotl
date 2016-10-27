#define ult1_TagDist 3400
#define ult2_TagDist 4000
#define ult3_TagDist 3300 // TODO: This must be adjusted.

#define ARRAY_SIZE 20
#define SPEED 140


//read all 3 sensor data - save data somewhere - return boolean to tell if an object was deteced
bool readSensors(SensorData *sensorData) {
	bool sensor1, sensor2, sensor3;
	unsigned short dist1, dist2, dist3;

	dist1 = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
	delay(1);
  
	dist2 = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
	delay(1);
  
	dist3 = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);
	delay(1);

  //serialDebug("Dist1: "+ String(dist1));
  //serialDebug(" Dist2: "+ String(dist2));
  //serialDebug(" Dist3: "+ String(dist3));
  //serialDebug("\n");

  sensor1 = dist1 < ult1_TagDist;
  sensor2 = dist2 < ult2_TagDist;
  sensor3 = dist3 < ult3_TagDist;
  
	if (sensor1 && sensor2) {
    //serialDebug("Block detected!\n");
		sensorData->sensor1 = dist1;
		sensorData->sensor2 = dist2;
		sensorData->sensor3 = dist3;
		sensorData->time = millis();

		return true;
	} else {
		heigthBetweenSensorAndBelt = 5000;
		lengthBetweenSensors = 8800;
		return false;
	}
}



//based on the collected data (so far) create the object
void handleSensorData(Package *package, SensorData buffer[], int bufferStartIndex, int bufferCount) {
	unsigned short sensor1, sensor2, sensor3;
	unsigned short packageTime;

  // TODO: Find a better solution.
	sensor1 = findMode(buffer, bufferStartIndex, bufferCount, 1);
	sensor2 = findMode(buffer, bufferStartIndex, bufferCount, 2);
	sensor3 = findMode(buffer, bufferStartIndex, bufferCount, 3);

  unsigned long startTime = buffer[bufferStartIndex].time;
  unsigned long endTime = buffer[(bufferStartIndex + bufferCount-1) % SENSOR_BUFFER_SIZE].time;

  package->middleTime = startTime - endTime;
	package->height = heigthBetweenSensorAndBelt - sensor1;
	package->width = lengthBetweenSensors - sensor2 - sensor3;
	packageTime = endTime - startTime;
	package->length = packageTime / 10 * SPEED ;

}

int findMode(SensorData buffer[], int bufferStartIndex, int bufferCount, byte sensor) {
	unsigned short countArray[ARRAY_SIZE][2];
	unsigned short currentLength = 0;
	unsigned short sensorData = 0;
	bool wasDetected = false;
  int bufferIndex = 0;

	for (int i = bufferStartIndex; i < bufferStartIndex + bufferCount; i++) {
    bufferIndex = i % SENSOR_BUFFER_SIZE;
    
		if (sensor == 1)
			sensorData = buffer[bufferIndex].sensor1;
		else if (sensor == 2)
			sensorData = buffer[bufferIndex].sensor2;
		else if (sensor == 3)
			sensorData = buffer[bufferIndex].sensor3;

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
	unsigned short largestCount = 0;

	for (int i = 0; i < ARRAY_SIZE; i++) {
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
