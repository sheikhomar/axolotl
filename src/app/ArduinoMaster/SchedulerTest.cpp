// 
// 
// 

#include "SchedulerTest.h"
#include "Defines.h"
#include "Scheduler.h"
#include "Utilities.h"
#include "SerialController.h"


void runAllTests() {
	//OBS! Do not run all tests at once -- we got mem leak somewhere in the system
	//Run removePackageTest on it's own
	findNextPackageForColourTest();
	findNextPackageForPlacementTest();
	convertMeasuredValueToMillimetresTest();
	findPackageByIdTest();
	resetPackageTest();
	//removePackageTest();
	serialDebug("Done executing "); 
	serialDebugLN("all tests");
}

void findNextPackageForColourTest() {
	//Arrange
	PackageCollection packagesOneColourRequest;
	PackageCollection packagesTwoColourRequest;
	PackageCollection packagesNoColourRequest;
	Package *packageOneColour;
	Package *packageTwoColour;
	Package *packageNoColour;

	//Third package have requested a colour, third should be returned first
	packageEmulator(&packagesOneColourRequest, 1, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesOneColourRequest, 2, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesOneColourRequest, 3, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, 1);
	
	//First and third package have requested a colour, first should be returned first
	packageEmulator(&packagesTwoColourRequest, 4, 2000, 3000, 4000, 300, COLOUR_REQUESTED, 1);
	packageEmulator(&packagesTwoColourRequest, 5, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesTwoColourRequest, 6, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, 1);
	
	//No package is waiting for colour, should return null
	packageEmulator(&packagesNoColourRequest, 7, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesNoColourRequest, 8, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesNoColourRequest, 9, 8000, 12000, 16000, 1200, COLOUR_NOT_REQUESTED, 1);

	//Act
	packageOneColour = findNextPackageForColour(&packagesOneColourRequest);
	packageTwoColour = findNextPackageForColour(&packagesTwoColourRequest);
	packageNoColour = findNextPackageForColour(&packagesNoColourRequest);

	//Assert
	if (packageOneColour->id != 3) {
		serialDebugLN("findNextPackageForColourTest failed case 1");
	}
	if (packageTwoColour->id != 4) {
		serialDebugLN("findNextPackageForColourTest failed case 2");
	}
	if (packageNoColour != NULL) {
		serialDebugLN("findNextPackageForColourTest failed case 3");
	}
}

void findNextPackageForPlacementTest() {
	//Arrange
	PackageCollection packagesOneBinRequest;
	PackageCollection packagesTwoBinRequest;
	PackageCollection packagesOneBinRequestSecondTime;
	PackageCollection packagesTwoBinRequestSecondTime;
	PackageCollection packagesNoBinRequest;
	Package *packageOneBin;
	Package *packageTwoBin;
	Package *packageOneBinSecondTime;
	Package *packageTwoBinSecondTime;
	Package *packageNoBin;

	//Third package has requested a bin, should return package 3
	packageEmulator(&packagesOneBinRequest, 1, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesOneBinRequest, 2, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, BIN_NOT_REQUESTED);
	packageEmulator(&packagesOneBinRequest, 3, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, BIN_REQUESTED);

	//Second and third package have requested a bin, should return package 5
	packageEmulator(&packagesTwoBinRequest, 4, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_NOT_REQUESTED);
	packageEmulator(&packagesTwoBinRequest, 5, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, BIN_REQUESTED);
	packageEmulator(&packagesTwoBinRequest, 6, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, BIN_REQUESTED);

	//Second package has requested a bin again, should return package 8
	packageEmulator(&packagesOneBinRequestSecondTime, 7, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packagesOneBinRequestSecondTime, 8, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, BIN_REQUESTED_AGAIN);
	packageEmulator(&packagesOneBinRequestSecondTime, 9, 8000, 12000, 16000, 1200, COLOUR_NOT_REQUESTED, BIN_NOT_REQUESTED);

	//First and third package have requested a bin again, should return package 1
	packageEmulator(&packagesTwoBinRequestSecondTime, 10, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_REQUESTED_AGAIN);
	packageEmulator(&packagesTwoBinRequestSecondTime, 11, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_NOT_REQUESTED);
	packageEmulator(&packagesTwoBinRequestSecondTime, 12, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_REQUESTED);

	//No package has requested a bin, should return null
	packageEmulator(&packagesNoBinRequest, 13, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_NOT_REQUESTED);
	packageEmulator(&packagesNoBinRequest, 14, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_NOT_REQUESTED);
	packageEmulator(&packagesNoBinRequest, 15, 2000, 3000, 4000, 300, COLOUR_REQUESTED, BIN_NOT_REQUESTED);

	//Act
	packageOneBin = findNextPackageForPlacement(&packagesOneBinRequest);
	packageTwoBin = findNextPackageForPlacement(&packagesTwoBinRequest);
	packageOneBinSecondTime = findNextPackageForPlacement(&packagesOneBinRequestSecondTime);
	packageTwoBinSecondTime = findNextPackageForPlacement(&packagesTwoBinRequestSecondTime);
	packageNoBin = findNextPackageForPlacement(&packagesNoBinRequest);

	//Assert
	if (packageOneBin->id != 3) {
		serialDebugLN("findNextPackageForPlacementTest failed case 1");
	}
	if (packageTwoBin->id != 5) {
		serialDebugLN("findNextPackageForPlacementTest failed case 2");
	}
	if (packageOneBinSecondTime->id != 8) {
		serialDebugLN("findNextPackageForPlacementTest failed case 3");
	}
	if (packageTwoBinSecondTime->id != 10) {
		serialDebugLN("findNextPackageForPlacementTest failed case 4");
	}
	if (packageNoBin != NULL) {
		serialDebugLN("findNextPackageForPlacementTest failed case 5");
	}
}

void convertMeasuredValueToMillimetresTest() {
	//Arrange
	unsigned short input;
	byte result;
	byte expectedResult;

	//Act and assert
	//Min unsigned short value tested
	input = 0;
	expectedResult = 0;
	result = convertMeasuredValueToMillimetres(input);
	if (result != expectedResult) {
		serialDebug("convertMeasuredValueToMillimetres failed case 1: ");
		serialDebug(String(result));
		serialDebug(" != ");
		serialDebugLN(String(expectedResult));
	}

	//Max unsigned short value tested
	input = 65535;
	expectedResult = 655;
	result = convertMeasuredValueToMillimetres(input);
	if (result != expectedResult) {
		serialDebug("convertMeasuredValueToMillimetres failed case 2: ");
		serialDebug(String(result));
		serialDebug(" != ");
		serialDebugLN(String(expectedResult));
	}

	//Random unsigned short value tested
	input = 23232;
	expectedResult = 232;
	result = convertMeasuredValueToMillimetres(input);
	if (result != expectedResult) {
		serialDebug("convertMeasuredValueToMillimetres failed case 3: ");
		serialDebug(String(result));
		serialDebug(" != ");
		serialDebugLN(String(expectedResult));
	}
}

void findPackageByIdTest() {
	//Arrange
	PackageCollection packages;
	short packageIdFound;
	short packageIdNotFound;

	//Packages with different ID is created
	packageEmulator(&packages, 1, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packages, 2, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packages, 3, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, 1);

	//Act
	//Should return index 2
	packageIdFound = findPackageById(&packages, 3);
	//Should return -1 because id is not found
	packageIdNotFound = findPackageById(&packages, 4);

	//Assert
	if (packageIdFound != 2) {
		serialDebug("findPackageByIdTest");
		serialDebugLN("failed case 1");
	}
	if (packageIdNotFound != NO_PACKAGE_FOUND) {
		serialDebug("findPackageByIdTest");
		serialDebugLN("failed case 2");
	}
}

void resetPackageTest() {
	//Arrange
	PackageCollection packages;
	Package pack1;
	Package pack2;

	packageEmulator(&packages, 1, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packages, 2, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, BIN_REQUESTED);

	pack1 = packages.items[0];
	pack2 = packages.items[1];

	//Act
	//Reseting two different packages should have the same values when reset
	resetPackage(&(pack1));
	resetPackage(&(pack2));

	//Assert
	if (pack1.bin != BIN_NOT_REQUESTED || pack1.colour != COLOUR_NOT_REQUESTED ||
		pack1.height != 0 || pack1.id != 0 || pack1.length != 0 || pack1.middleTime != 0 || pack1.width != 0) {
		serialDebugLN("resetPackageTest failed case 1");
	}
	if (pack2.bin != BIN_NOT_REQUESTED || pack2.colour != COLOUR_NOT_REQUESTED ||
		pack2.height != 0 || pack2.id != 0 || pack2.length != 0 || pack2.middleTime != 0 || pack2.width != 0) {
		serialDebugLN("resetPackageTest failed case 2");
	}
}

void removePackageTest() {
	//Arrange
	PackageCollection packages1;
	PackageCollection packages2;

	packageEmulator(&packages1, 1, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, 1);
	packageEmulator(&packages1, 2, 4000, 6000, 8000, 600, COLOUR_NOT_REQUESTED, BIN_NOT_REQUESTED);
	packageEmulator(&packages1, 3, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, BIN_REQUESTED);

	packageEmulator(&packages2, 4, 2000, 3000, 4000, 300, COLOUR_NOT_REQUESTED, BIN_REQUESTED);
	packageEmulator(&packages2, 5, 4000, 6000, 8000, 600, COLOUR_BLUE, BIN_NOT_REQUESTED);
	packageEmulator(&packages2, 6, 8000, 12000, 16000, 1200, COLOUR_REQUESTED, BIN_REQUESTED);

	//Act and assert
	//Checking the result after the first package is removed
	//serialDebugLN(String(packages1.items[0].id));
	removePackage(&packages1, 0);
	if (packages1.count != 2 || packages1.items[0].id != 2 || packages1.items[1].id != 3) {
		serialDebugLN("removePackageTest failed case 1");
	}
	//Checking the result when given an invalid index which previous was valid
	removePackage(&packages1, 2);
	if (packages1.count != 2 || packages1.items[0].id != 2 || packages1.items[1].id != 3) {
		serialDebugLN("removePackageTest failed case 2");
	}
	//Checking the result when given an invalid index which never was true
	removePackage(&packages1, NO_PACKAGE_FOUND);
	if (packages1.count != 2 || packages1.items[0].id != 2 || packages1.items[1].id != 3) {
		serialDebugLN("removePackageTest failed case 3");
	}
	
	//Checking the result when removing first package
	removePackage(&packages2, 0);
	if (packages2.count != 2 || packages2.items[0].id != 5 || packages2.items[1].id != 6) {
		serialDebugLN("removePackageTest failed case 4");
	}
	//Checking the result when given the lowest value possible
	removePackage(&packages2, -32768);
	if (packages2.count != 2 || packages2.items[0].id != 5 || packages2.items[1].id != 6) {
		serialDebugLN("removePackageTest failed case 5");
	}
	//Checking the result when given the highest value possible
	removePackage(&packages2, 32767);
	if (packages2.count != 2 || packages2.items[0].id != 5 || packages2.items[1].id != 6) {
		serialDebugLN("removePackageTest failed case 6");
	}
	//Checking the result after removing the last package
	removePackage(&packages2, 1);
	if (packages2.count != 1 || packages2.items[0].id != 5) {
		serialDebugLN("removePackageTest failed case 7");
	}
	//Checking the result after removing the last package on the list
	removePackage(&packages2, 0);
	if (packages2.count != 0) {
		serialDebugLN("removePackageTest failed case 8");
	}
	//Checking the results after trying to remove an item on an empty list
	removePackage(&packages2, 0);
	if (packages2.count != 0) {
		serialDebugLN("removePackageTest failed case 9");
	}
}

//Helper function
void packageEmulator(PackageCollection *packages, int _nextPackageId, int height, int width, int length, int middletime, byte colour, byte bin) {
		if (packages->count >= PACKAGE_BUFFER_SIZE) {
			String masterString = "Reached max packages _ ";
			serialDebug(masterString);
			die(String(packages->count));
		}
		
		Package *package = &(packages->items[packages->count]);
		packages->count += 1;
		
		package->id = _nextPackageId;
		
		package->height = height;
		package->width = width;
		package->length = length;
		package->middleTime = middletime;
		package->colour = colour;
		package->bin = bin;
}