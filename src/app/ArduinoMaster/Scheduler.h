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
#define FROM_ULT_TO_COLOUR_SENSOR_MS 450
#define FROM_ULT_TO_ARM1_MS 1092 
#define FROM_ULT_TO_ARM2_MS 1508 

#define SENSOR_ALLOWED_FALSE_POSITIVES 2
#define BLINKINTERVAL 50
#define LEDTOGGLE_LED LED_YELLOW

//Functions
void printPackages(PackageCollection *packages);
void sendPackageInfoToRaspberryPi(Package *package);
void resendPackageInfoToRaspberryPI(Package *package);
void pushArm(PackageCollection *packages);
void requestColourInformation(Package *package);
void resetPackage(Package *package);
void removePackage(PackageCollection *packages, int index);
void receiveData(PackageCollection *packages);
void sendData(PackageCollection *packages);
void resetSensorReading(SensorReading *reading);
void resetSensorData(SensorReading *r1, SensorReading *r2, SensorReading *r3);
bool readSensorsEx(SensorReading *r1, SensorReading *r2, SensorReading *r3);
void cleanBuffer(SensorReading *reading, short sensor);
void handlePackage(PackageCollection *packages, SensorReading *r1, SensorReading *r2, SensorReading *r3);
void runScheduler();
void checkBufferCount(unsigned short buffer1, unsigned short buffer2, unsigned short buffer3);
void handleSensorNoise(SensorReading *reading);
void lampToggle(int *val);
byte convertMeasuredValueToMillimetres(unsigned short number);
void debugLamp(int command);

#endif

