// Scheduler.h

#ifndef _SCHEDULER_h
#define _SCHEDULER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Defines
#define FROM_ULT_TO_COLOUR_SENSOR_MS 462
#define FROM_ULT_TO_ARM1_MS 1319
#define FROM_ULT_TO_ARM2_MS 1847

#define NOT_DETECTED_THRESHOLD 3

//Functions
void requestColourFromNXT(Package *package);
bool isColourInfoReady(Package *package);
bool isPackingAdviceReady(Package *package);
void sendPackageInfoToRaspberryPi(Package *package);
bool readPackingAdvice(Package *package);
bool pushArm(Package *package);
void requestColourInformation(Package *package);
void resetPackage(Package *package);
void resetPackages(Package packages[]);
void runScheduler();



#endif

