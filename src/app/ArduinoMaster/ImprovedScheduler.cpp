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

	SensorReading sensorBuffer1[SENSOR_BUFFER_SIZE];
	int sensorBuffer1Count = 0;
	bool sensor1IsDetecting = false;

	SensorReading sensorBuffer2[SENSOR_BUFFER_SIZE];
	int sensorBuffer2Count = 0;

	SensorReading sensorBuffer3[SENSOR_BUFFER_SIZE];
	int sensorBuffer3Count = 0;

	for (int i = 0; i < SENSOR_BUFFER_SIZE; i++) {
		sensorBuffer1[i].time = 0;
		sensorBuffer2[i].time = 0;
		sensorBuffer3[i].time = 0;
	}

	int notDetectedCount = 0;

	serialDebug("Resetting packages.\n");
	resetImprovedPackages(packages);
	serialDebug("Before entering while loop\n");

	while (true) {
		// delay(15);

		bool newPackageDetected1 = readSensor(sensorBuffer1, &sensorBuffer1Count, SENSOR_1);

		if(sensor1IsDetecting)

		if (newPackageDetected1) {
			// We have detected a new package in the conveyor belt

			if (sensorBuffer1Count == SENSOR_BUFFER_SIZE) {
				die("Panic! Buffer for sensor 1 data is full.");
			}

			sensorBuffer1Count++;
			notDetectedCount = 0;
			sensor1IsDetecting = true;
		}


		else {
			/*notDetectedCount++;

			if (sensorBuffer1Count > 0 && notDetectedCount1 > NOT_DETECTED_THRESHOLD) {
				// At this stage, we have collected distance information for a single package.
				// The function 'handleSensorData' builds an instance of Package based 
				// on the data in the sensorData.

				String temp = String(sensorBuffer1Count);
				serialDebug("Sensor size: " + temp + "\n");

				if (packageCount == PACKAGE_BUFFER_SIZE) {
					die("Panic! Buffer for packages is full.");
				}

				// Find current package
				Package *p = &packages[(packageStartIndex + packageCount) % PACKAGE_BUFFER_SIZE];

				//serialDebug("Working with package: " + String((packageStartIndex + packageCount) % PACKAGE_BUFFER_SIZE) + " .\n");

				// Prepare next package
				packageCount++;

				serialDebug("Package count: " + String(packageCount) + "\n");

				// Fill Package object using collected sensor data
				handleSensorData(p, sensorBuffer, sensorBufferStartIndex, sensorBufferCount);

				serialDebug("Package: " + String(p->width) + " x " + String(p->height) + " x " + String(p->length) + "\n" + "\n");

				// Empty buffer for sensor data
				sensorBufferCount = 0;
			}*/
		}
	}
}
