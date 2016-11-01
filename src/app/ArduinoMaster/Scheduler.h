// Scheduler.h

#ifndef _SCHEDULER_h
#define _SCHEDULER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Defines
#define COLOUR_RED 0
#define COLOUR_GREEN 1
#define COLOUR_BLUE 2
#define COLOUR_YELLOW 3
#define COLOUR_WHITE 6
#define COLOUR_BLACK 7
#define COLOUR_UNKNOWN 128
#define COLOUR_NONE 254
#define COLOUR_REQUESTED 255

#define FROM_ULT_TO_COLOUR_SENSOR_MS 2098
#define FROM_ULT_TO_ARM1_MS 2564
#define FROM_ULT_TO_ARM2_MS 3497

#define NOT_DETECTED_THRESHOLD 3

//Functions
void requestColourFromNXT(Package *package);
bool isColourInfoReady(Package *package);
bool isPackingAdviceReady(Package *package);
void sendPackageInfoToRaspberryPi(Package *package);
bool readPackingAdvice(Package *package);
bool pushArm(Package *package);
void finalisePackage(Package *package);
void resetPackage(Package *package);
void resetPackages(Package packages[]);
void runScheduler();



#endif

