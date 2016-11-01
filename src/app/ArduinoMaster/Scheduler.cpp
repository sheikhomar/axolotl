// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "LEDController.h"
#include "UltrasoundSensor.h"
#include "ObjectIdentification.h"

#include "Scheduler.h"


void requestColourFromNXT(Package *package) {
    byte buf[] = { 0 };

    // Request colour information from the NXT
    serialSendData(NXT, buf, 0, COMM_NXT_GET_COLOUR);

    package->colour = COLOUR_REQUESTED;
}

bool isColourInfoReady(Package *package) {
    // Check serial to see if we have received data from NXT
    byte buf[] = { 0 };
    client clientInfo = serialReadData(buf, 1);
    if (clientInfo == Arduino) {
        package->colour = buf[0];
        serialDebug("Colour from NXT: " + String(buf[0]) + "\n");
        return true;
    }

    return false;
}

bool isPackingAdviceReady(Package *package) {
    // Check serial to see if we have received data from Raspberry Pi
    return false;
}


void sendPackageInfoToRaspberryPi(Package *package) {
    byte buf[] = { (byte)package->width, (byte)package->length, (byte)package->height, (byte)package->colour };
    serialSendData(RaspberryPi, buf, 4, 1);
}

bool readPackingAdvice(Package *package) {
    byte buf[1];
    serialReadData(buf, 1);
    // TODO: Ensure that the received data is correct before we continue.
    package->bin = buf[0];
    return true;
}

bool pushArm(Package *package) {
    if (package->bin != -1) {
        unsigned long currentTime = millis();
        unsigned long timeDiff = currentTime - package->middleTime;

        if (package->bin == 1 && timeDiff >= FROM_ULT_TO_ARM1_MS) {
            byte buf[1];
            serialSendData(NXT, buf, 0, COMM_NXT_PUSH_ARM1);
            return true;
        }
        else if (package->bin == 2 && timeDiff >= FROM_ULT_TO_ARM2_MS) {
            byte buf[1];
            serialSendData(NXT, buf, 0, COMM_NXT_PUSH_ARM2);
            return true;
        }
    }
    return false;
}

void requestColourInformation(Package *package) {
    if (package->colour == COLOUR_NONE) {
        unsigned long currentTime = millis();
        unsigned long timeDiff = currentTime - package->middleTime;

        if (timeDiff >= FROM_ULT_TO_COLOUR_SENSOR_MS) {
            requestColourFromNXT(package);
        }
    }
}

void resetPackage(Package *package) {
    package->id = 0;
    package->length = 0;
    package->width = 0;
    package->height = 0;
    package->colour = COLOUR_NONE;
    package->middleTime = 0;
    package->bin = -1;
}

void resetPackages(Package packages[]) {
    for (int i = 0; i < PACKAGE_BUFFER_SIZE; i++) {
        resetPackage(&packages[i]);
        packages[i].id = i + 1;
    }
}

void runScheduler() {
    serialDebug("Schedule started.\n");
    Package packages[PACKAGE_BUFFER_SIZE];
    int packageStartIndex = 0;
    int packageCount = 0;

    SensorData sensorBuffer[SENSOR_BUFFER_SIZE];
    int sensorBufferStartIndex = 0;
    int sensorBufferCount = 0;

    for (int i = 0; i < SENSOR_BUFFER_SIZE; i++) {
        sensorBuffer[i].time = 0;
    }

    int notDetectedCount = 0;
    int detectedCount = 0;

    serialDebug("Resetting packages.\n");
    resetPackages(packages);
    serialDebug("Before entering while loop\n");

    while (true) {
        delay(15);
        SensorData *sd = &sensorBuffer[(sensorBufferStartIndex + sensorBufferCount) % SENSOR_BUFFER_SIZE];
        bool newPackageDetected = readSensors(sd);

        if (newPackageDetected) {
            // We have detected a new package in the conveyor belt

            if (sensorBufferCount == SENSOR_BUFFER_SIZE) {
                die("Panic! Buffer for sensor data is full.");
            }

            sensorBufferCount++;
            notDetectedCount = 0;
        }
        else {
            notDetectedCount++;

            if (sensorBufferCount > 0 && notDetectedCount > NOT_DETECTED_THRESHOLD) {
                // At this stage, we have collected distance information for a single package.
                // The function 'handleSensorData' builds an instance of Package based 
                // on the data in the sensorData.

                String temp = String(sensorBufferCount);
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
            }
        }

        if (packageCount > 0) {
            Package *p = &packages[packageStartIndex];
            requestColourInformation(p);

            if (isColourInfoReady(p)) {
                pushArm(p);

                // Remove package from the buffer
                packageStartIndex = (packageStartIndex + 1) % PACKAGE_BUFFER_SIZE;
                packageCount--;
                resetPackage(p);
            }
        }

    }//while
}