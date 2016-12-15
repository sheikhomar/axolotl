// SchedulerTest.h

#ifndef _SCHEDULERTEST_h
#define _SCHEDULERTEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Include
#include "Defines.h"

//Functions
void runAllTests();
void findNextPackageForColourTest();
void findNextPackageForPlacementTest();
void convertMeasuredValueToMillimetresTest();
void findPackageByIdTest();
void resetPackageTest();
void removePackageTest();

//Helper function
void packageEmulator(PackageCollection *packages, int _nextPackageId, int height, int width, int length, int middletime, byte colour, byte bin);

#endif

