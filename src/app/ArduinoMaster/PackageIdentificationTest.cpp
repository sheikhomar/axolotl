// 
// 
// 
#include "Defines.h"
#include "PackageIdentification.h"
#include "SerialController.h"


#include "PackageIdentificationTest.h"


void runPackageIdentificationTest() {
	checkReadingTest();
	checkForFailedSensorTest();
	dequeueTest();
	setPackageInfoTest();
	setPackageInfoTestEx();
	findMedianTest();
	calcLength();
	calculateAverageSensorResultTest();
}


void checkReadingTest() {
	bool b1, b2, b3, b4, b5, b6, b7;

	b1 = checkReading(ULT_TOP_SENSOR, ULT_TOP_TAG_DIST-50);
	b2 = checkReading(ULT_TOP_SENSOR, ULT_TOP_TAG_DIST - 1);
	b3 = checkReading(ULT_TOP_SENSOR, ULT_TOP_TAG_DIST);
	b4 = checkReading(ULT_TOP_SENSOR, ULT_TOP_TAG_DIST + 1);
	b5 = checkReading(ULT_TOP_SENSOR, ULT_TOP_TAG_DIST +50);

	if (!(b1 && b2) || b3 || b4 || b5 || b6) {
		serialDebugLN("Package Identification FAILED - check readingTest");
	}
}

void checkForFailedSensorTest() {
	delay(TRANSPORT_TIME+60);
	PackageIdentificationState state = createPackageidStateMockup();
	unsigned long virtualEndtime;

	virtualEndtime = millis()- TRANSPORT_TIME - 50;
	checkForFailedSensor(virtualEndtime,&state);
	if (state.leftSensorBuffer.result == 0 || state.rightSensorBuffer.result == 0 || state.topSensorBuffer.result == 0) {
		serialDebugLN("Package Identification FAILED - failedSensor check 1");
	}

	state = createPackageidStateMockup();
	virtualEndtime = millis();
	checkForFailedSensor(virtualEndtime, &state);
	if (state.leftSensorBuffer.result == 0 || state.rightSensorBuffer.result == 0 || state.topSensorBuffer.result == 0) {
		serialDebugLN("Package Identification FAILED - failedSensor check 2");
	}

	state = createPackageidStateMockup();
	virtualEndtime = millis() + 15;
	checkForFailedSensor(virtualEndtime, &state);
	if (state.leftSensorBuffer.result == 0 || state.rightSensorBuffer.result == 0 || state.topSensorBuffer.result == 0) {
		serialDebugLN("Package Identification FAILED - failedSensor check 3");
	}
}

void dequeueTest() {
	PackageIdentificationState state = createPackageidStateMockup();
	
	SensorResultQueue queue = state.leftSensorResultQueue;
	unsigned short before = queue.data[queue.count - 2].result;
	unsigned short newElement = queue.data[queue.count - 1].result;
	int queueCount = queue.count;

	dequeue(&queue);

	if (queueCount - 1 != queue.count) {
		serialDebugLN("PackageIdentification Error - did not count right");
	}

	if (queue.data[queue.count].result != newElement) {
		serialDebugLN("PackageIdentification Error - did not move element");
		serialDebugLN(String(before));
		serialDebugLN(String(newElement));
		serialDebugLN(String(queue.data[queue.count].result));

	}

	dequeue(&queue);
	dequeue(&queue);
}

void setPackageInfoTest() {
	//sets the correct information based on excel
	SensorResult resL, resT, resR;
	Package package;

	resL.endTime = 5;
	resL.startTime = 1;
	resL.result = 4000;

	resT.endTime = 5;
	resT.startTime = 2;
	resT.result = 3500;

	resR.endTime = 5;
	resR.startTime = 3;
	resR.result = 3200;

	//Set package values
	package.id = 256;
	package.bin = 256;
	package.colour = COLOUR_UNKNOWN;
	package.height = 256;
	package.length = 256;
	package.width = 256;
	package.middleTime = 256;

	setPackageInfo(&package, &resL, &resT, &resR);

	//Value check
	if (package.id != 256) {
		serialDebugLN("Package Identification FAILED - changed id");
	}

	if (package.bin != 256) {
		serialDebugLN("Package Identification FAILED - changed bin");
	}

	if (package.colour != COLOUR_UNKNOWN) {
		serialDebugLN("Package Identification FAILED - changed colour");
	}

	if (package.height != 950) {
		serialDebugLN("Package Identification FAILED - wrong height");
	}

	if (package.length != 54) {
		serialDebugLN("Package Identification FAILED - wrong length");
	}

	if (package.width != 4600) {
		serialDebugLN("Package Identification FAILED - wrong width");
	}

	if (package.middleTime-1.5 > 0.1 && package.middleTime - 1.5 < -0.1) {
		serialDebugLN("Package Identification FAILED - wrong middletime");
	}
}

void setPackageInfoTestEx() {
	//sets the correct information based on excel
	SensorResult resL, resT, resR;
	Package package;

	resL.endTime = 5;
	resL.startTime = 1;
	resL.result = 5000;

	resT.endTime = 5;
	resT.startTime = 2;
	resT.result = 5000;

	resR.endTime = 5;
	resR.startTime = 3;
	resR.result = 5000;

	//Set package values
	package.id = 256;
	package.bin = 256;
	package.colour = COLOUR_UNKNOWN;
	package.height = 256;
	package.length = 256;
	package.width = 256;
	package.middleTime = 256;

	setPackageInfo(&package, &resL, &resT, &resR);

	if (package.height <= 0) {
		serialDebugLN("Package Identification FAILED - invalid height");
	}

	if (package.width <= 0) {
		serialDebugLN("Package Identification FAILED - invalid width");
	}
}

void findMedianTest() {
	findMedianTestHelper(5, 1, 6);
	findMedianTestHelper(0,0,0);
	findMedianTestHelper(1,0,2);
	findMedianTestHelper(1, 2, 0);
	findMedianTestHelper(5,4,6);
	findMedianTestHelper(66000,65999,66001);
	findMedianTestHelper(1, 1, 3);
	findMedianTestHelper(1, 1, 0);
}

void findMedianTestHelper(unsigned long median, unsigned long valB, unsigned long valC) {
	unsigned long returnedMedian = findMedian(median, valB, valC);

	if (returnedMedian != median) {
		serialDebugLN("Package Identification FAILED - incorrect median");
		serialDebug("Should be ");
		serialDebug(String(median));
		serialDebug(" but was ");
		serialDebugLN(String(returnedMedian));
	}
}

void calcLength() {
	SensorResult res;
	unsigned long dist, returnedDist;

	res.endTime = 25.;
	res.startTime = 15.;
	dist = (res.endTime - res.startTime) / 10 * SPEED_CONVEYOR;

	returnedDist = calcLength(&res);

	if (returnedDist - dist > 0.1  && returnedDist - dist < -0.1) {
		serialDebugLN("Package Identification FAILED - incorrect dist 1");
	}

	res.endTime = 0;
	res.startTime = 0;
	dist = (res.endTime - res.startTime) / 10 * SPEED_CONVEYOR;

	returnedDist = calcLength(&res);

	if (returnedDist - dist > 0.1  && returnedDist - dist < -0.1) {
		serialDebugLN("Package Identification FAILED - incorrect dist 2");
	}

	res.endTime = 15;
	res.startTime = 25;
	dist = (res.endTime - res.startTime) / 10 * SPEED_CONVEYOR;

	returnedDist = calcLength(&res);

	if (returnedDist - dist > 0.1  && returnedDist - dist < -0.1) {
		serialDebugLN("Package Identification FAILED - incorrect dist 3");
	}
}

void calculateAverageSensorResultTest() {
	ReadingCollection dataCollection;
	unsigned short res = 0;
	int i;

	//check  1..5
	dataCollection.readings[0] = 1;
	dataCollection.readings[1] = 2;
	dataCollection.readings[2] = 3;
	dataCollection.readings[3] = 4;
	dataCollection.readings[4] = 5;
	dataCollection.count = 5;

	res = calculateAverageSensorResult(&dataCollection);

	if (res != 3) {
		serialDebug("Package Identification FAILED - incorrect avg 1");
		serialDebug(" was ");
		serialDebugLN(String(res));
	}

	//check  2,4,6,10
	dataCollection.readings[0] = 2;
	dataCollection.readings[1] = 4;
	dataCollection.readings[2] = 6;
	dataCollection.readings[3] = 14;
	dataCollection.count = 4;

	res = calculateAverageSensorResult(&dataCollection);

	if (res != 6) {
		serialDebug("Package Identification FAILED - incorrect avg 2");
		serialDebug(" was ");
		serialDebugLN(String(res));
	}

	//check  1..6
	dataCollection.readings[0] = 1;
	dataCollection.readings[1] = 2;
	dataCollection.readings[2] = 3;
	dataCollection.readings[3] = 4;
	dataCollection.readings[4] = 5;
	dataCollection.readings[5] = 6;
	dataCollection.count = 6;

	res = calculateAverageSensorResult(&dataCollection);

	if (res != 3) {
		serialDebug("Package Identification FAILED - incorrect avg 3");
		serialDebug(" was ");
		serialDebugLN(String(res));
	}

	//0 elements
	dataCollection.readings[0] = 25;
	dataCollection.count = 0;

	res = calculateAverageSensorResult(&dataCollection);

	if (res != 0) {
		serialDebug("Package Identification FAILED - incorrect avg 4");
		serialDebug(" was ");
		serialDebugLN(String(res));
	}
}


PackageIdentificationState createPackageidStateMockup() {
	PackageIdentificationState PIDstate;

	SensorResultQueue queue;
	SensorResult sensorRes,sensorRes2;

	sensorRes.endTime = millis() - 50;
	sensorRes.startTime = millis() - 150;
	sensorRes.result = 50;

	sensorRes2.endTime = 1500;
	sensorRes2.startTime = 2500;
	sensorRes2.result = 150;

	queue.count = 2;
	queue.data[0] = sensorRes;
	queue.data[1] = sensorRes2;

	ReadingCollection readingColl;
	readingColl.count = 3;
	readingColl.readings[0] = 1;
	readingColl.readings[1] = 2;
	readingColl.readings[2] = 3;

	SensorBuffer buffer;
	buffer.data = readingColl;
	buffer.endTime = millis() - 10;
	buffer.startTime = millis() - 30;
	buffer.isReady = true; 
	buffer.result = 25;

	SlidingWindowInformation swInfo;
	swInfo.count = 2;
	swInfo.currentValue = 5;
	swInfo.measurements[0] = 4;
	swInfo.measurements[1] = 7;
	swInfo.sumOfMeasurements = 11;

	PIDstate.leftSensorBuffer = buffer;
	PIDstate.leftSensorResultQueue = queue;
	PIDstate.rightSensorBuffer = buffer;
	PIDstate.rightSensorResultQueue = queue;
	PIDstate.slidingLeftSensor = swInfo;
	PIDstate.slidingRightSensor = swInfo;
	PIDstate.slidingTopSensor = swInfo;
	PIDstate.topSensorBuffer = buffer;
	PIDstate.topSensorResultQueue = queue;

	return PIDstate;
}