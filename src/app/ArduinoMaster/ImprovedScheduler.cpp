// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "LEDController.h"
#include "UltrasoundSensor.h"
#include "ObjectIdentification.h"

#include "ImprovedScheduler.h"

void resetImprovedPackage(Package *package) {
	package->id = 0;
	package->length = 0;
	package->width = 0;
	package->height = 0;
	package->colour = COLOUR_NONE;
	package->middleTime = 0;
	package->bin = -1;
}

void resetImprovedPackages(Package packages[]) {
	for (int i = 0; i < PACKAGE_BUFFER_SIZE; i++) {
		resetImprovedPackage(&packages[i]);
		packages[i].id = i + 1;
	}
}

void resetSensorReading(SensorReading *reading) {
	reading->startTime = 0;
	reading->acceptEndTime = 0;
	reading->unacceptedEndTime = 0;
	reading->bufferCount = 0;
}

void runImprovedScheduler() {
	serialDebug("Scheduler getting ready.\n");
	Package packages[PACKAGE_BUFFER_SIZE];
	int packageStartIndex = 0;
	int packageCount = 0;

	SensorReading sensor1;
	resetSensorReading(&sensor1);

	SensorReading sensor2;
	resetSensorReading(&sensor2);

	SensorReading sensor3;
	resetSensorReading(&sensor3);

	resetImprovedPackages(packages);
	serialDebug("Scheduler ready!\nYou can now put blocks on the belt.\n\n");

	while (true) {
		delay(5);
		bool readyForHandling2 = readSensor(&sensor2, SENSOR_2);
		delay(5);
		bool readyForHandling1 = readSensor(&sensor1, SENSOR_1);
		delay(5);
		bool readyForHandling3 = readSensor(&sensor3, SENSOR_3);

		checkBufferCount(sensor1.bufferCount);
		checkBufferCount(sensor2.bufferCount);
		checkBufferCount(sensor3.bufferCount);

		
		if (readyForHandling1 && readyForHandling2 && readyForHandling3) {
			// At this stage, we have collected distance information for a single package.
			// The function 'handleSensorData' builds an instance of Package based 
			// on the data in the sensorData.

			//serialDebugLN("Sensor 1 - Count: " + String(sensor1.bufferCount));
			//serialDebugLN("Sensor 2 - Count: " + String(sensor2.bufferCount));
			//serialDebugLN("Sensor 3 - Count: " + String(sensor3.bufferCount));

			cleanBuffer(&sensor1, SENSOR_1);
			cleanBuffer(&sensor2, SENSOR_2);
			cleanBuffer(&sensor3, SENSOR_3);

			//serialDebugLN("Sensor 1 (Clean) - Count: " + String(sensor1.bufferCount));
			//serialDebugLN("Sensor 2 (Clean) - Count: " + String(sensor2.bufferCount));
			//serialDebugLN("Sensor 3 (Clean) - Count: " + String(sensor3.bufferCount));

			// Find current package
			Package *p = &packages[(packageStartIndex + packageCount) % PACKAGE_BUFFER_SIZE];

			// Prepare next package
			packageCount++;

			// Fill Package object using collected sensor data
			handleSensorReadings(p, &sensor1, &sensor2, &sensor3);

			//serialDebug("Package: " + String(p->width) + " x " + String(p->height) + " x " + String(p->length) + "\n");

			// Empty buffer for sensor data
			resetSensorReading(&sensor1);
			resetSensorReading(&sensor2);
			resetSensorReading(&sensor3);

			serialDebug("\n\n\n");
		}
	}
}

void checkBufferCount(short buffCount) {
	if (buffCount == SENSOR_BUFFER_SIZE) 
		die("Panic! Buffer for sensor 1 data is full.");
}

void cleanBuffer(SensorReading *reading, short sensor) {
	unsigned short tagDist;
	if (sensor == SENSOR_1)
		tagDist = ult1_TagDist;
	else if (sensor == SENSOR_2)
		tagDist = ult2_TagDist;
	else if (sensor == SENSOR_3)
		tagDist = ult3_TagDist;
	
	for (int i = 0; i < reading->bufferCount; i++) {
		if (reading->sensorReadingBuffer[i] > tagDist) {
			reading->bufferCount = reading->bufferCount - 1;
			for (int j = i; j < reading->bufferCount; j++) {
				reading->sensorReadingBuffer[j] = reading->sensorReadingBuffer[j + 1];
			}
			i--;
		}
	}
}

