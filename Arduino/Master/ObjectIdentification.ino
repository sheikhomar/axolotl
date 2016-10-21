#define  ult1_TagDist = 45
#define ult2_TagDist = 60
#define ult3_TagDist = 33




//read all 3 sensor data - return bool if tagging | save data somewhere
bool readSensors() {
	bool sensor1, sensor2, sensor3;
	double distance;

	distance = GetUltDistance(ult1_TrigPin, ult1_echoPin, false);
	sensor1 = distance < ult1_TagDist;

	distance = GetUltDistance(ult2_TrigPin, ult2_echoPin, false);
	sensor2 = distance < ult2_TagDist;

	distance = GetUltDistance(ult3_TrigPin, ult3_echoPin, false);
	sensor3 = distance < ult3_TagDist;

	return (sensor1 && sensor2);
}



//based on the collected data (so far) create the object
