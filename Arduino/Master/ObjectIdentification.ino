#define ult1_TagDist 45
#define ult2_TagDist 60
#define ult3_TagDist 33




//read all 3 sensor data - return bool if tagging | save data somewhere
void readSensors(int data[][3], int i) {
	bool sensor1, sensor2, sensor3;
	double dist1, dist2, dist3;


	dist1 = GetUltDistance(ult1_TrigPin, ult1_echoPin, false);
	sensor1 = dist1 < ult1_TagDist;

	dist2 = GetUltDistance(ult2_TrigPin, ult2_echoPin, false);
	sensor2 = dist2 < ult2_TagDist;

	dist3 = GetUltDistance(ult3_TrigPin, ult3_echoPin, false);
	sensor3 = dist3 < ult3_TagDist;

	if (sensor1 && sensor2) {
		data[i][0] = dist1;
		data[i][1] = dist2;
		data[i][2] = dist3;
	}
}



//based on the collected data (so far) create the object
void handleSensorData(int data[][3], int dimensionSize) {

}

int findMode(int data[][3], int dimensionSize, int dimension) {
	int countArray[100][2];
	int currentLength = 0;
	bool wasDetected = false;

	for (int i = 0; i <= dimensionSize; i++) {
		for (int j = 0; j <= currentLength; j++) {
			if (data[i][0] == countArray[j][0]) {
				countArray[j][1]++;
				wasDetected = true;
			}
		}
		if (!wasDetected) {
			countArray[currentLength][0] = data[i][0];
			countArray[currentLength][1] = 1;
		}
	}
}