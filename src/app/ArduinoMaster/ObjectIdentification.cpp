// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "ObjectIdentification.h"
#include "KalmanFilter.h"

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
		if (reading->falseCount < NOT_DETECTED_THRESHOLD) {
			addReading(reading, dist);
			checkAndIncrement(reading, sensorTag);
		}

		if (reading->falseCount == 0) {
			reading->endTime = millis();
		}

		if (reading->falseCount >= NOT_DETECTED_THRESHOLD) {
			return true;
		}
		return false;
	}
}

short makeReading(int whichSensor) {
	short dist;

	if (whichSensor == ULT_TOP_SENSOR) {
		dist = GetUltDistance(ULT_TOP_TRIG_PIN, ULT_TOP_ECHO_PIN, false);
	}
	else if (whichSensor == ULT_RIGHT_SENSOR) {
		dist = GetUltDistance(ULT_RIGHT_TRIG_PIN, ULT_RIGHT_ECHO_PIN, false);
	}
	else if (whichSensor == ULT_LEFT_SENSOR) {
		dist = GetUltDistance(ULT_LEFT_TRIG_PIN, ULT_LEFT_ECHO_PIN, false);
	}
	else {
		die("ERROR - No real sensor.");
	}

	return dist;
}

bool checkReading(int whichSensor, int dist) {
	bool bVal;

	if (whichSensor == ULT_TOP_SENSOR) {
		bVal = dist < ult1_TagDist;
	}
	else if (whichSensor == ULT_RIGHT_SENSOR) {
		bVal = dist < ult2_TagDist;
	}
	else if (whichSensor == ULT_LEFT_SENSOR) {
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

void initObjectIdentification(ObjectIdentificationState *state) {
    initKalmanFilter(&state->topSensor, 5000, 200, 4000, 10);
    initKalmanFilter(&state->rightSensor, 5000, 200, 4000, 10);
    initKalmanFilter(&state->leftSensor, 5000, 200, 4000, 10);
}

void runIdentification(ObjectIdentificationState *state, PackageCollection *packages) {
    bool leftSensorTag = performReading(&state->leftSensor, &state->leftSensorBuffer.data, ULT_LEFT_SENSOR);
    bool topSensorTag = performReading(&state->topSensor, &state->topSensorBuffer.data, ULT_TOP_SENSOR);
    bool rightSensorTag = performReading(&state->rightSensor, &state->rightSensorBuffer.data, ULT_RIGHT_SENSOR);
    
    setSensorResult(leftSensorTag, &state->leftSensorBuffer, LEFT_SENSOR_CHECK_DISTANCE);
    setSensorResult(topSensorTag, &state->topSensorBuffer, TOP_SENSOR_CHECK_DISTANCE);
    setSensorResult(rightSensorTag, &state->rightSensorBuffer, RIGHT_SENSOR_CHECK_DISTANCE);

    // TODO: Package generation.
}

void setSensorResult(bool tag, SensorBuffer *sensorBuffer, unsigned short sensorCheckDistance) {
    if (!tag && sensorBuffer->data.count > 0) {
        if (sensorBuffer->data.count > CORRECT_AMOUNT_THRESHOLD) {
            sensorBuffer->result = calculateSensorResult(&sensorBuffer->data, sensorCheckDistance);
            sensorBuffer->isReady = true;
        }

        sensorBuffer->data.count = 0;
    }

    sensorBuffer->isReady = false;
}

unsigned short calculateSensorResult(ReadingCollection *collection, unsigned short checkDistance) {
    unsigned short bestValue = 0;
    unsigned short bestNumberOfNeighbours = 0;

    int loopingBound = collection->count;
    if (loopingBound > SENSOR_READINGS_SIZE) {
        loopingBound = SENSOR_READINGS_SIZE;
    }
    for (int i = 0; i < loopingBound; i++) {
        unsigned short currentValue = collection->readings[i];
        unsigned short currentNumberOfNeighbours = 0;

        for (int j = 0; j < loopingBound; j++) {
            unsigned short anotherValue = collection->readings[i];
            unsigned short diff = abs(currentValue - anotherValue);
            if (i != j && diff >= checkDistance) {
                currentNumberOfNeighbours += 1;
            }
        }

        if (currentNumberOfNeighbours >= bestNumberOfNeighbours) {
            // TODO: Consider using average to handle the case where best and current are equal
            bestValue = currentValue;
            bestNumberOfNeighbours = currentNumberOfNeighbours;
        }
    }

    return bestValue;
}

bool performReading(KalmanFilterInformation *kfi, ReadingCollection *collection, int whichSensor) {
    delay(5);
    double measurement = (double)makeReading(whichSensor);

    updateKalmanFilter(kfi, measurement);

    bool tag = checkReading(whichSensor, kfi->currentEstimate);
    if (tag) {
        double estimate = kfi->currentEstimate;
        addItemToCollection(collection, estimate);
    }

    return tag;
}

void addItemToCollection(ReadingCollection *collection, double estimate) {
    // TODO: Potential type cast problem
    collection->readings[collection->count % SENSOR_READINGS_SIZE] = (unsigned short)estimate;
    collection->count += 1;
}