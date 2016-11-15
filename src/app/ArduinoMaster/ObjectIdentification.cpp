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

bool readSensor(SensorReading *reading, int whichSensor) {
	bool sensorTag = false;
	unsigned short dist;

	dist = makeReading(whichSensor);
	sensorTag = checkReading(whichSensor, dist);

	if (reading->bufferCount == 0 && sensorTag) {
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
		checkAndIncrement(reading, sensorTag);

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
	reading->bufferCount = (reading->bufferCount) + 1;
}

void checkAndIncrement(SensorReading *reading, bool sensorTag) {
	
	if (sensorTag) {
		reading->falseCount = 0;
	}
	else if (reading->falseCount == NOT_DETECTED_THRESHOLD)
	{
		reading->falseCount = NOT_DETECTED_THRESHOLD;
	}
	else {
		reading->falseCount = reading->falseCount + 1;
	}
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
	package->middleTime = (sensor1MiddleTime); //todo, we're assuming only one middletime atm
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