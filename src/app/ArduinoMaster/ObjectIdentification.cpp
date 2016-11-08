// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "ObjectIdentification.h"

#define ARRAY_SIZE 20
#define SPEED_CONVEYOR 140


//read all 3 sensor data - save data somewhere - return boolean to tell if an object was deteced
bool readSensors(SensorData *sensorData) {
	bool sensor1, sensor2, sensor3;
	unsigned short dist1, dist2, dist3;

	dist2 = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
	delay(1);
	dist1 = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
	delay(1);
	dist3 = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);

	//serialDebug("Dist1: "+ String(dist1));
	//serialDebug(" Dist2: "+ String(dist2));
	//serialDebug(" Dist3: "+ String(dist3));
	//serialDebug("\n");

	sensor1 = dist1 < ult1_TagDist;
	sensor2 = dist2 < ult2_TagDist;
	sensor3 = dist3 < ult3_TagDist;

	if (sensor1) {
		//serialDebug("Block detected!\n");
		sensorData->sensor1 = dist1;
		sensorData->sensor2 = dist2;
		sensorData->sensor3 = dist3;
		sensorData->time = millis();

		return true;
	}
	else {
		return false;
	}
}

bool readSensor(SensorReading *reading, int whichSensor) {
	bool sensor = false;
	unsigned short dist;

	dist = makeReading(whichSensor);
	sensor = checkReading(whichSensor, dist);

	if (reading->bufferCount == 0 && sensor) {
		addReading(reading, dist);
		reading->startTime = millis();
		reading->endTime = millis();
		return false;
	} 
	else if (reading->bufferCount == 0){
		return false;
	} 
	else {
		addReading(reading, dist);
		checkAndIncrement(reading, sensor);

		if (reading->falseCount == 0) {
			reading->endTime = millis();
		}
		if (reading->falseCount == NOT_DETECTED_THRESHOLD) {
			return true;
		}
		return false;
	}
}

short makeReading(int whichSensor) {
	short dist;

	if (whichSensor == SENSOR_1) {
		dist = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
	}
	else if (whichSensor == SENSOR_2) {
		dist = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
	}
	else if (whichSensor == SENSOR_3) {
		dist = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);
	}
	else {
		die("ERROR - No real sensor.");
	}

	return dist;
}

bool checkReading(int whichSensor, int dist) {
	bool bVal;

	if (whichSensor == SENSOR_1) {
		bVal = dist < ult1_TagDist;
	}
	else if (whichSensor == SENSOR_2) {
		bVal = dist < ult2_TagDist;
	}
	else if (whichSensor == SENSOR_3) {
		bVal = dist < ult3_TagDist;
	}
	else {
		die("ERROR - No real sensor.");
	}

	return bVal;
}

void addReading(SensorReading *reading, unsigned short dist) {
	reading->sensorReadingBuffer[reading->bufferCount] = dist;
	reading->bufferCount = reading->bufferCount + 1;
}

void checkAndIncrement(SensorReading *reading, bool bVal) {
	
	if (bVal)
		reading->falseCount = 0;
	else if (reading->falseCount == NOT_DETECTED_THRESHOLD)
		reading->falseCount = NOT_DETECTED_THRESHOLD;
	else
		reading->falseCount = reading->falseCount + 1;
}

void handleSensorReadings(Package *package, SensorReading *sensor1, SensorReading *sensor2, SensorReading *sensor3){
	//Finding package length
	unsigned long lengthBasedOnSensor1 = findLength(sensor1);
	unsigned long lengthBasedOnSensor2 = findLength(sensor2);
	unsigned long lengthBasedOnSensor3 = findLength(sensor3);
	package->length = (lengthBasedOnSensor1 + lengthBasedOnSensor2 + lengthBasedOnSensor3 ) / 3;

	//Normalizing sensor data
	unsigned long sensor1Length = normalizeSensorData(sensor1);
	unsigned long sensor2Length = normalizeSensorData(sensor2);
	unsigned long sensor3Length = normalizeSensorData(sensor3);

	//Finding width and height
	package->height = HEIGHT_BETWEEN_SENSOR_AND_BELT - sensor1Length;
	package->width = LENGTH_BETWEEN_SENSORS - sensor2Length - sensor3Length;

	printPackageSize(package);

	//Finding middle time
	unsigned long sensor1MiddleTime = findMiddleTime(sensor1);
	unsigned long sensor2MiddleTime = findMiddleTime(sensor2);
	unsigned long sensor3MiddleTime = findMiddleTime(sensor3);
	package->middleTime = (sensor1MiddleTime + sensor2MiddleTime + sensor3MiddleTime) / 3;
}

void printPackageSize(Package *package) {
	serialDebug("Length: ");
	//serialDebug("(" + String(lengthBasedOnSensor1));
	//serialDebug(" + " + String(lengthBasedOnSensor2) + " + ");
	//serialDebug(String(lengthBasedOnSensor3) + ") / 3 = ");
	serialDebug(String(package->length) + "\n");

	serialDebug("Width: ");
	//serialDebug(String(LENGTH_BETWEEN_SENSORS) + " - ");
	//serialDebug(String(sensor2Length) + " - ");
	//serialDebug(String(sensor3Length) + " = ");
	serialDebug(String(package->width) + "\n");

	serialDebug("Height: ");
	//serialDebug("Height: " + String(HEIGHT_BETWEEN_SENSOR_AND_BELT));
	//serialDebug(" - " + String(sensor1Length) + " = ");
	serialDebug(String(package->height) + "\n");
}

unsigned long normalizeSensorData(SensorReading *sensor) {
	unsigned long total; 

	for (int i = 0; i < sensor->bufferCount; i++) {
		total += sensor->sensorReadingBuffer[i];
	}
	unsigned long result = total / sensor->bufferCount;

	return result;
}

unsigned long findMiddleTime(SensorReading *sensor) {
	unsigned long result = sensor->startTime + ((sensor->endTime - sensor->startTime) / 2);

	return result;
}

unsigned long findLength(SensorReading *sensor) {
	unsigned long packageTime = sensor->endTime - sensor->startTime;
	unsigned long result = packageTime / 10 * SPEED_CONVEYOR;

	//serialDebug("packageTime = endTime - startTime = " + String(sensor->endTime) + " - " + String(sensor->startTime) + " = " + String(packageTime) + "\n");
	//serialDebug("result = packageTime / 10 * SPEED_CONVEYOR = " + String(packageTime) + " / 10 * 140 = " + String(result) + "\n");
	return result;
}

//based on the collected data (so far) create the object
void handleSensorData(Package *package, SensorData buffer[], int bufferStartIndex, int bufferCount) {
    unsigned long sensor1, sensor2, sensor3;
    unsigned long packageTime;

    // TODO: Find a better solution.
    sensor1 = findAverage(buffer, bufferStartIndex, bufferCount, 1);
    sensor2 = findAverage(buffer, bufferStartIndex, bufferCount, 2);
    sensor3 = findAverage(buffer, bufferStartIndex, bufferCount, 3);


    //serialDebug("Sensor1: "+ String(sensor1));
    //serialDebug(" Sensor2: "+ String(sensor2));
    //serialDebug(" Sensor3: "+ String(sensor3));
    //serialDebug("\n");


    unsigned long startTime = buffer[bufferStartIndex].time;
    unsigned long endTime = buffer[(bufferStartIndex + bufferCount - 1) % SENSOR_BUFFER_SIZE].time;

    package->middleTime = startTime + ((endTime - startTime) / 2);
    //package->height = heigthBetweenSensorAndBelt - sensor1;
    //package->width = lengthBetweenSensors - sensor2 - sensor3;
    package->height = 2000;
    package->width = 3200;
    packageTime = endTime - startTime;
    package->length = packageTime / 10 * SPEED_CONVEYOR;

    // TODO: Fix this
    if (package->length > 500 && package->length < 4000) {
        package->length = 3200;
    }
    else if (package->length > 4000 && package->length < 6400) {
        package->length = 6400;
    }
    else if (package->length > 6400) {
        package->length = 12800;
    }
}

unsigned long findAverage(SensorData buffer[], int startIndex, int bufferSize, byte whichSensor) {
    unsigned long total = 0;
    int bufferIndex = 0;
    for (int i = startIndex; i < startIndex + bufferSize; i++) {
        bufferIndex = i % SENSOR_BUFFER_SIZE;
        if (whichSensor == 1)
            total += buffer[bufferIndex].sensor1;
        else if (whichSensor == 2)
            total += buffer[bufferIndex].sensor2;
        else if (whichSensor == 3)
            total += buffer[bufferIndex].sensor3;
    }

    return total / bufferSize;
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
            mostCommonValue = (mostCommonValue + countArray[i][0]) / 2;
        }
    }

    return mostCommonValue;
}