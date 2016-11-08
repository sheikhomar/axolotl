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

void runImprovedScheduler() {
	serialDebug("Schedule started.\n");
	Package packages[PACKAGE_BUFFER_SIZE];
	int packageStartIndex = 0;
	int packageCount = 0;

	SensorReading sensorBuffer1[IMPROVED_BUFFER_SIZE];
	int sensorBuffer1Count = 0;

	SensorReading sensorBuffer2[IMPROVED_BUFFER_SIZE];
	int sensorBuffer2Count = 0;

	SensorReading sensorBuffer3[IMPROVED_BUFFER_SIZE];
	int sensorBuffer3Count = 0;

	for (int i = 0; i < IMPROVED_BUFFER_SIZE; i++) {
		sensorBuffer1[i].time = 0;
		sensorBuffer2[i].time = 0;
		sensorBuffer3[i].time = 0;
	}

	serialDebug("Resetting packages.\n");
	resetImprovedPackages(packages);
	serialDebug("Before entering while loop\n\n");

	while (true) {
		delay(5);
		bool readyForHandling2 = readSensor(sensorBuffer2, &sensorBuffer2Count, SENSOR_2);
		delay(5);
		bool readyForHandling1 = readSensor(sensorBuffer1, &sensorBuffer1Count, SENSOR_1);
		delay(5);
		bool readyForHandling3 = readSensor(sensorBuffer3, &sensorBuffer3Count, SENSOR_3);

		//serialDebug("S1: " + String(sensorBuffer1[sensorBuffer1Count].sensorReading) + " Count: " + String(sensorBuffer1Count) +"\n");
			//+ " S2: " + String(sensorBuffer2[sensorBuffer2Count].sensorReading) + " S3: " + String(sensorBuffer3[sensorBuffer3Count].sensorReading) + "\n");

		checkBufferCounts(sensorBuffer1Count, sensorBuffer2Count, sensorBuffer3Count);
		
		if (readyForHandling1 && readyForHandling2 && readyForHandling3) {
			// At this stage, we have collected distance information for a single package.
			// The function 'handleSensorData' builds an instance of Package based 
			// on the data in the sensorData.

			serialDebugLN("Sensor 1 - Count: " + String(sensorBuffer1Count));
			serialDebugLN("Sensor 2 - Count: " + String(sensorBuffer2Count));
			serialDebugLN("Sensor 3 - Count: " + String(sensorBuffer3Count));


			cleanBuffer(sensorBuffer1, &sensorBuffer1Count, SENSOR_1);
			cleanBuffer(sensorBuffer2, &sensorBuffer2Count, SENSOR_2);
			cleanBuffer(sensorBuffer3, &sensorBuffer3Count, SENSOR_3);

			serialDebugLN("Sensor 1 (Clean) - Count: " + String(sensorBuffer1Count));
			/*for (int i = 0; i < sensorBuffer1Count; i++) {
				serialDebugLN("" + String(sensorBuffer1[i].sensorReading));
			}*/
			serialDebugLN("Sensor 2 (Clean) - Count: " + String(sensorBuffer2Count));
			//for (int i = 0; i < sensorBuffer2Count; i++) {
			//	serialDebugLN("" + String(sensorBuffer2[i].sensorReading));
			//}
			serialDebugLN("Sensor 3 (Clean) - Count: " + String(sensorBuffer3Count));
			/*for (int i = 0; i < sensorBuffer3Count; i++) {
				serialDebugLN("" + String(sensorBuffer3[i].sensorReading));
			}*/

			// Find current package
			Package *p = &packages[(packageStartIndex + packageCount) % PACKAGE_BUFFER_SIZE];

			// Prepare next package
			packageCount++;
			//serialDebug("Package count: " + String(packageCount) + "\n");

			// Fill Package object using collected sensor data
			handleSensorReadings(p, sensorBuffer1, sensorBuffer1Count, sensorBuffer2, sensorBuffer2Count, sensorBuffer3, sensorBuffer3Count);

			//serialDebug("Package: " + String(p->width) + " x " + String(p->height) + " x " + String(p->length) + "\n");

			// Empty buffer for sensor data
			sensorBuffer1Count = 0;
			sensorBuffer2Count = 0;
			sensorBuffer3Count = 0;
			serialDebug("\n\n\n");
		}
	}
}

void checkBufferCounts(short BuffCount1, short BuffCount2, short BuffCount3) {
		if (BuffCount1 == IMPROVED_BUFFER_SIZE) {
			die("Panic! Buffer for sensor 1 data is full.");
		}
		if (BuffCount2 == IMPROVED_BUFFER_SIZE) {
			die("Panic! Buffer for sensor 2 data is full.");
		}
		if (BuffCount3 == IMPROVED_BUFFER_SIZE) {
			die("Panic! Buffer for sensor 3 data is full.");
	}
}

void cleanBuffer(SensorReading sensorBuffer[], int *bufferCount, short sensor) {
	for (int i = 0; i < *bufferCount; i++) {
		if (sensor == SENSOR_1) {
			if (sensorBuffer[i].sensorReading > ult1_TagDist) {
				*bufferCount = *bufferCount - 1;
				for (int j = i; j < *bufferCount; j++) {
					sensorBuffer[j] = sensorBuffer[j + 1];
				}
				i--;
			}

		} else if (sensor == SENSOR_2) {
			if (sensorBuffer[i].sensorReading > ult2_TagDist) {
				*bufferCount = *bufferCount - 1;
				for (int j = i; j < *bufferCount; j++) {
					sensorBuffer[j] = sensorBuffer[j + 1];
				}
				i--;
			}
		} else if (sensor == SENSOR_3) {
			if (sensorBuffer[i].sensorReading > ult3_TagDist) {
				*bufferCount = *bufferCount - 1;
				for (int j = i; j < *bufferCount; j++) {
					sensorBuffer[j] = sensorBuffer[j + 1];
				}
				i--;
			}
		}
	}
}
