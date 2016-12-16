// PackageIdentificationTest.h

#ifndef _PACKAGEIDENTIFICATIONTEST_h
#define _PACKAGEIDENTIFICATIONTEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Includes
#include "Utilities.h"


//Functions
PackageIdentificationState createPackageidStateMockup();
void runPackageIdentificationTest();
void checkReadingTest();
void checkForFailedSensorTest();
void dequeueTest();
void setPackageInfoTest();
void setPackageInfoTestEx();
void findMedianTest();
void findMedianTestHelper(unsigned long median, unsigned long valB, unsigned long valC);
void calcLength();
void calculateAverageSensorResultTest();

#endif

