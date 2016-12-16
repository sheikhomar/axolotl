// Scheduler.h

#ifndef _SCHEDULER_h
#define _SCHEDULER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else 
	#include "WProgram.h"
#endif

#include "Defines.h"

//Defines
#define FROM_ULT_TO_COLOUR_SENSOR_MS 390
#define FROM_ULT_TO_COLOUR_SENSOR_MS_DELAY 100
#define FROM_ULT_TO_ARM1_MS 1092 
#define FROM_ULT_TO_ARM2_MS 1508 

#define SENSOR_ALLOWED_FALSE_POSITIVES 2
#define BLINKINTERVAL 50
#define LEDTOGGLE_LED LED_YELLOW

//Functions
void sendPackageInfoToRaspberryPi(Package *package);
void pushArm(PackageCollection *packages);
void requestColourInformation(Package *package);
void resetPackage(Package *package);
Package *findNextPackageForColour(PackageCollection *packages);
Package *findNextPackageForPlacement(PackageCollection *packages);
void removePackage(PackageCollection *packages, int index);
void receiveData(PackageCollection *packages);
void sendData(PackageCollection *packages);
void runScheduler();
byte convertMeasuredValueToMillimetres(unsigned short number);
short findNextColorRequested(PackageCollection *packages, short id);


#endif

