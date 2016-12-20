// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "PackageIdentification.h"
#include "KalmanFilter.h"
#include "SlidingWindowFilter.h"

// Global variables
unsigned int ___nextPackageId = 1;

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
		bVal = dist < ULT_TOP_TAG_DIST;
	}
	else if (whichSensor == ULT_RIGHT_SENSOR) {
		bVal = dist < ULT_RIGHT_TAG_DIST;
	}
	else if (whichSensor == ULT_LEFT_SENSOR) {
		bVal = dist < ULT_LEFT_TAG_DIST;
	}
	else {
		die("ERROR - No real sensor.");
	}

	return bVal;
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

void initPackageIdentification(PackageIdentificationState *state) {
	#if KALMAN_OR_SLIDING
		initKalmanFilter(&state->kalmanTopSensor, 2000, 400, 10000, 40);
		initKalmanFilter(&state->kalmanRightSensor, 2000, 400, 10000, 40);
		initKalmanFilter(&state->kalmanLeftSensor, 2000, 400, 10000, 40);
	#else
		initSlidingWindowInformation(&state->slidingTopSensor);
		initSlidingWindowInformation(&state->slidingRightSensor);
		initSlidingWindowInformation(&state->slidingLeftSensor);
	#endif

    initSensorBuffer(&state->topSensorBuffer);
    initSensorBuffer(&state->rightSensorBuffer);
    initSensorBuffer(&state->leftSensorBuffer);

    state->leftSensorResultQueue.count = 0;
    state->topSensorResultQueue.count = 0;
    state->rightSensorResultQueue.count = 0;
}

void initSensorBuffer(SensorBuffer *buffer) {
    buffer->isReady = false;
    buffer->result = 0;
    buffer->data.count = 0;
}

void resetPackage2(Package *package) {
    package->id = 0;
    package->length = 0;
    package->width = 0;
    package->height = 0;
    package->colour = COLOUR_NOT_REQUESTED;
    package->bin = BIN_NOT_REQUESTED;
    package->middleTime = 0;
}

void checkForFailedSensor(unsigned long endtime, PackageIdentificationState *state) {
	if (millis() >= (endtime + TRANSPORT_TIME)) {
		if (state->leftSensorResultQueue.count > 0) {
			dequeue(&state->leftSensorResultQueue);
		}

		if (state->topSensorResultQueue.count > 0) {
			dequeue(&state->topSensorResultQueue);
		}

		if (state->rightSensorResultQueue.count > 0) {
			dequeue(&state->rightSensorResultQueue);
		}
		
		serialDebugLN("\nWaited too long. Flushing queues.\n");
	}
}

SensorResult dequeue(SensorResultQueue *queue) {
    SensorResult item = queue->data[0];

    for (int i = 0; i < queue->count; i++) {
        if (i < queue->count - 1) {
            queue->data[i] = queue->data[i + 1];
        }
    }

    queue->count -= 1;

    return item;
}

void setPackageInfo(Package *package, SensorResult *leftResult, SensorResult *topResult, SensorResult *rightResult) {
    package->height = HEIGHT_BETWEEN_SENSOR_AND_BELT - topResult->result;
    package->width = LENGTH_BETWEEN_SENSORS - rightResult->result - leftResult->result;

	//Serial.print("\t\tHeight: \t" + String(package->height));
	//Serial.print("\tWidth: \t" + String(package->width));

    unsigned long leftLength = calcLength(leftResult);
    unsigned long topLength = calcLength(topResult);
    unsigned long rightLength = calcLength(rightResult);

    package->length = 0.95 * findMedian(leftLength, topLength, rightLength);

	//Serial.println("\tLength: \t" + String(package->length));

    unsigned long leftMiddleTime = (leftResult->endTime - leftResult->startTime) / 2;
    unsigned long topMiddleTime = (topResult->endTime - topResult->startTime) / 2;
    unsigned long rightMiddleTime = (rightResult->endTime - rightResult->startTime) / 2;

	unsigned long largestStartTime = 0; 

	if (leftResult->startTime >= largestStartTime)
		largestStartTime = leftResult->startTime;
	if (topResult->startTime >= largestStartTime)
		largestStartTime = topResult->startTime;
	if (rightResult->startTime >= largestStartTime)
		largestStartTime = rightResult->startTime;

    package->middleTime = largestStartTime + findMedian(leftMiddleTime, topMiddleTime, rightMiddleTime);

	serialDebug("h: ");
	serialDebug(String(package->height));
	serialDebug(" w: ");
	serialDebug(String(package->width));
	serialDebug(" l: ");
	serialDebugLN(String(package->length));
	//serialDebug(" mt: ");
	//serialDebugLN(String(package->middleTime));
}

unsigned long findMedian(unsigned long left, unsigned long top, unsigned long right) {
    unsigned long lengthArray[3];
    lengthArray[0] = left;
    lengthArray[1] = right;
    lengthArray[2] = top;

    sort(lengthArray, 3);
    
    return lengthArray[1];
}

// Source: http://hwhacks.com/2016/05/03/bubble-sorting-with-an-arduinoc-application/
// Bubble sort
void sort(unsigned long a[], int size) {
    for (int i = 0; i<(size - 1); i++) {
        for (int o = 0; o<(size - (i + 1)); o++) {
            if (a[o] > a[o + 1]) {
                unsigned long t = a[o];
                a[o] = a[o + 1];
                a[o + 1] = t;
            }
        }
    }
}

unsigned long calcLength(SensorResult *result) {
    unsigned long diff = result->endTime - result->startTime;
    return diff / 10 * SPEED_CONVEYOR;
}

void queueResult(SensorBuffer *sensorBuffer, SensorResultQueue *queue) {
    if (sensorBuffer->isReady) {
        if (queue->count >= SENSOR_RESULT_SIZE) {
            die("Sensor result size!");
        }
        queue->data[queue->count].result = sensorBuffer->result;
        queue->data[queue->count].startTime = sensorBuffer->startTime;
        queue->data[queue->count].endTime = sensorBuffer->endTime;

        queue->count += 1;
        initSensorBuffer(sensorBuffer);
    }
}

void createSensorResult(bool packageDetected, SensorBuffer *sensorBuffer, unsigned short sensorCheckDistance, String sensor) {
    if (!packageDetected && sensorBuffer->data.count > 0) {
        if (sensorBuffer->data.count > CORRECT_AMOUNT_THRESHOLD) {
            sensorBuffer->result = calculateAverageSensorResult(&sensorBuffer->data);
            sensorBuffer->isReady = true;
        }
		//Serial.print("\t");calculateDensitySensorResult
		//Serial.print(sensor);
		//Serial.print("\tdens:\t" + String(sensorBuffer->result));
		//Serial.print("\tmin:\t" + String(calculateMinimumSensorResult(&sensorBuffer->data)));
		//Serial.print("\tavg:\t" + String(calculateDensitySensorResult(&sensorBuffer->data)));
		
		//serialDebugLN(sensor + " buffercount = " + String(sensorBuffer->data.count));

        sensorBuffer->data.count = 0;
    }
}

unsigned short calculateDensitySensorResult(ReadingCollection *collection, long checkDistance) {
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
            unsigned short anotherValue = collection->readings[j];
			long diff = currentValue - anotherValue;
			long posDiff = abs(diff);

            if (i != j && diff <= checkDistance) {
                currentNumberOfNeighbours += 1;
            }
        }

        if (currentNumberOfNeighbours >= bestNumberOfNeighbours) {
            bestValue = currentValue;
            bestNumberOfNeighbours = currentNumberOfNeighbours;
        }
    }
	
    return bestValue;
}

unsigned short calculateAverageSensorResult(ReadingCollection *collection) {
	long result = 0; 
	int loopingBound = collection->count;

	if (loopingBound > SENSOR_READINGS_SIZE) {
		loopingBound = SENSOR_READINGS_SIZE;
	}

	for (int i = 0; i < loopingBound; i++) {
		result += collection->readings[i];
	}
	return (unsigned short)(result / loopingBound);
}

unsigned short calculateMinimumSensorResult(ReadingCollection *collection) {
	short result = collection->readings[0];
	int loopingBound = collection->count;

	if (loopingBound > SENSOR_READINGS_SIZE) {
		loopingBound = SENSOR_READINGS_SIZE;
	}

	for (int i = 1; i < loopingBound; i++) {
		if (result > collection->readings[i]) {
			result = collection->readings[i];
		}
	}

	return result;
}


bool performReading(PackageIdentificationState *state, SensorBuffer *buffer, int whichSensor) {
	delay(1);
    double measurement = (double) makeReading(whichSensor);
	bool tag;
	double estimate; 

	#if KALMAN_OR_SLIDING
		if (whichSensor == ULT_TOP_SENSOR) {
			updateKalmanFilter(&state->kalmanTopSensor, measurement);
			tag = checkReading(whichSensor, state->kalmanTopSensor.currentEstimate);
			estimate = state->kalmanTopSensor.currentEstimate;
		}
		else if (whichSensor == ULT_RIGHT_SENSOR) {
			updateKalmanFilter(&state->kalmanRightSensor, measurement);
			tag = checkReading(whichSensor, state->kalmanRightSensor.currentEstimate);
			estimate = state->kalmanRightSensor.currentEstimate;
		}
		else if (whichSensor == ULT_LEFT_SENSOR) {
			updateKalmanFilter(&state->kalmanLeftSensor, measurement);
			tag = checkReading(whichSensor, state->kalmanLeftSensor.currentEstimate);
			estimate = state->kalmanLeftSensor.currentEstimate;
		}
	#else
		if (whichSensor == ULT_TOP_SENSOR) {
			updateSlidingWindowFilter(&state->slidingTopSensor, measurement);
			if (state->slidingTopSensor.count >= SLIDING_WINDOW_K) {
				tag = checkReading(whichSensor, state->slidingTopSensor.currentValue);
				estimate = state->slidingTopSensor.currentValue;
			}
		}
		else if (whichSensor == ULT_RIGHT_SENSOR) {
			updateSlidingWindowFilter(&state->slidingRightSensor, measurement);
			if (state->slidingTopSensor.count >= SLIDING_WINDOW_K) {
				tag = checkReading(whichSensor, state->slidingRightSensor.currentValue);
				estimate = state->slidingRightSensor.currentValue;
			}
		}
		else if (whichSensor == ULT_LEFT_SENSOR) {
			updateSlidingWindowFilter(&state->slidingLeftSensor, measurement);			
			if (state->slidingTopSensor.count >= SLIDING_WINDOW_K) {
				tag = checkReading(whichSensor, state->slidingLeftSensor.currentValue);
				estimate = state->slidingLeftSensor.currentValue;
			}
		}
	#endif

    if (tag) {
        if (buffer->data.count == 0) {
            buffer->startTime = millis();
        }

        buffer->endTime = millis();
        addItemToCollection(&buffer->data, estimate);
    }

    return tag;
}

void addItemToCollection(ReadingCollection *collection, double estimate) {
    collection->readings[collection->count % SENSOR_READINGS_SIZE] = (unsigned short) estimate;
    collection->count += 1;
}

void runIdentification(PackageIdentificationState *state, PackageCollection *packages) {
	bool leftSensorTag = performReading(state, &state->leftSensorBuffer, ULT_LEFT_SENSOR);
	bool topSensorTag = performReading(state, &state->topSensorBuffer, ULT_TOP_SENSOR);
	bool rightSensorTag = performReading(state, &state->rightSensorBuffer, ULT_RIGHT_SENSOR);

	createSensorResult(leftSensorTag, &state->leftSensorBuffer, LEFT_SENSOR_CHECK_DISTANCE, "Left ");
	createSensorResult(topSensorTag, &state->topSensorBuffer, TOP_SENSOR_CHECK_DISTANCE, "Top ");
	createSensorResult(rightSensorTag, &state->rightSensorBuffer, RIGHT_SENSOR_CHECK_DISTANCE, "Right");

	queueResult(&state->leftSensorBuffer, &state->leftSensorResultQueue);
	queueResult(&state->topSensorBuffer, &state->topSensorResultQueue);
	queueResult(&state->rightSensorBuffer, &state->rightSensorResultQueue);

	if (state->leftSensorResultQueue.count > 0)
		checkForFailedSensor(state->leftSensorResultQueue.data[0].endTime, state);
	if (state->topSensorResultQueue.count > 0)
		checkForFailedSensor(state->topSensorResultQueue.data[0].endTime, state);
	if (state->rightSensorResultQueue.count > 0)
		checkForFailedSensor(state->rightSensorResultQueue.data[0].endTime, state);

	if (state->leftSensorResultQueue.count > 0 &&
		state->topSensorResultQueue.count > 0 &&
		state->rightSensorResultQueue.count > 0) {

		// Create new package
		Package *package = &(packages->items[packages->count]);
		packages->count = packages->count + 1;
		resetPackage2(package);
		package->id = ___nextPackageId;
		___nextPackageId += 1;

		serialDebug("\n\n\nNew package with ID: #");
		serialDebugLN(String(package->id));

		setPackageInfo(package,
			&dequeue(&state->leftSensorResultQueue),
			&dequeue(&state->topSensorResultQueue),
			&dequeue(&state->rightSensorResultQueue)
		);
	}
}