// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"
#include "ObjectIdentification.h"

#include "Scheduler.h"

// Global variables
unsigned int __nextPackageId = 1;

void sendPackageInfoToRaspberryPi(Package *package) {
    byte buf[] = { 0, 0, 0, 0 };

    //buf[0] = (byte)package->width / 1600;
    //buf[1] = (byte)package->length / 1600;
    //buf[2] = (byte)package->height / 1600;
    //buf[3] = package->colour;

    buf[0] = 2;
    buf[1] = 4;
    buf[2] = 2;
    buf[3] = package->colour;

    serialSendData(RaspberryPi, buf, 4, 'p');
    serialDebug("\n");

    package->bin == BIN_REQUESTED;
}

// Loop through all packages
//   If package can be pushed
//      If time to push
//          Push
//          Remove package
//      If Bin is Wrong Then delete
//          Remove package
// End Loop
void pushArm(PackageCollection *packages) {
    for (int i = 0; i < packages->count; i++) {
        Package package = packages->items[i];
        if (package.bin != BIN_NOT_REQUESTED && package.bin != BIN_REQUESTED) {
            unsigned long currentTime = millis();
            unsigned long timeDiff = currentTime - package.middleTime;

            if (package.bin == 1 && timeDiff >= FROM_ULT_TO_ARM1_MS) {
                serialSendData(NXT, COMM_NXT_PUSH_ARM1);
                removePackage(packages, i);
                i--;
            }
            else if (package.bin == 2 && timeDiff >= FROM_ULT_TO_ARM2_MS) {
                serialSendData(NXT, COMM_NXT_PUSH_ARM2);
                removePackage(packages, i);
                i--;
            }
            else if (package.bin != 1 && package.bin != 2) {
                serialDebugLN("Wrong bin PID: ");
                serialDebugLN(String(package.id));
                removePackage(packages, i);
                i--;
            }
        }
    }
}

void requestColourInformation(Package *package) {
    if (package->colour == COLOUR_NOT_REQUESTED) {
        unsigned long currentTime = millis();
        unsigned long timeDiff = currentTime - package->middleTime;

        if (timeDiff >= FROM_ULT_TO_COLOUR_SENSOR_MS) {
            serialDebug("\nColour requested for PID: ");
            serialDebugLN(String(package->id));

            // Request colour information from the NXT
            serialSendData(NXT, COMM_NXT_GET_COLOUR);

            package->colour == COLOUR_REQUESTED;
        }
    }
}

void resetPackage(Package *package) {
    package->id = 0;
    package->length = 0;
    package->width = 0;
    package->height = 0;
    package->colour = COLOUR_NOT_REQUESTED;
    package->middleTime = 0;
    package->bin = BIN_NOT_REQUESTED;
}

void removePackage(PackageCollection *packages, int index) {
    // Remove package from the buffer
    for (int i = index; i < packages->count; i++) {
        packages->items[i] = packages->items[i + 1];
    }
}

void removeFirstPackage(PackageCollection *packages) {
    serialDebug("Removing package: ");
    serialDebugLN(String(packages->items[0].id));
    for (int i = 0; i < packages->count; i++) {
        packages->items[i] = packages->items[i + 1];
    }
    packages->count = packages->count - 1;
}


// Read serial
// If Not For Us then 
//   Return
// Else
//   If Command = Colour
//      Set colour on the first package
//   Else Command = BinPacking
//      Move the first package to push array
//   End If
// End If
void receiveData(PackageCollection *packages) {
    // Check if we have received data
    byte buf[] = { 0 };
    int command = -1;
    client clientInfo = serialReadData(buf, 1, &command);

    if (packages->count == 0) {
        // Handle buffer noise in the serial.
        while (serialReadData(buf, 1, &command) != none) {
            return;
        }
    }

    if (clientInfo != Arduino) {
        return;
    }

    Package package = packages->items[0];

    if (command == COMM_ARDUINO_COLOUR_INFO) {
        // We have recieved data from NXT
        package.colour = buf[0];
        serialDebug("\nReceived Colour: ");
        serialDebugLN(String(package.colour));

        if (package.colour == COLOUR_BLACK || package.colour == COLOUR_UNKNOWN) {
            // If we get these colours then, we just remove the package
            serialDebugLN("\nBad colour");

            // Remove current package.
            removePackage(packages, 0);
        }
    }
    else if (command == COMM_PI_ADVICEPACKAGE) {
        // We have recieved data from Raspberry Pi
        package.bin = buf[0];
        serialDebug("\nReceived Bin: ");
        serialDebugLN(String(package.bin));
    }
}

// Loop through all packages
//   If colour information is requested?
//      Request colour information
//   Else If bin packing is requested?
//      Request bin packing
//   End If
// End Loop
void sendData(PackageCollection *packages) {
    for (int i = 0; i < packages->count; i++) {
        Package package = packages->items[i];
        if (package.colour == COLOUR_NOT_REQUESTED) {
            // Colour has not been requested
            requestColourInformation(&package);
        }
        else if (package.bin == BIN_NOT_REQUESTED) {
            // Packing advice not requested.
            sendPackageInfoToRaspberryPi(&package);
        }
    }
}


void resetSensorReading(SensorReading *reading) {
    reading->startTime = 0;
    reading->endTime = 0;
    reading->falseCount = 0;
    reading->bufferCount = 0;
    for (int i = 0; i < SENSOR_BUFFER_SIZE; i++) {
        reading->sensorReadingBuffer[i] = 0;
    }
}

void resetSensorData(SensorReading *r1, SensorReading *r2, SensorReading *r3) {
    resetSensorReading(r1);
    resetSensorReading(r2);
    resetSensorReading(r3);
}

bool readSensorsEx(SensorReading *r1, SensorReading *r2, SensorReading *r3) {
    //Reading sensors
    delay(5);
    bool readyForHandling2 = readSensor(r2, SENSOR_2);
    delay(5);
    bool readyForHandling1 = readSensor(r1, SENSOR_1);
    delay(5);
    bool readyForHandling3 = readSensor(r3, SENSOR_3);

    // checking buffer bound
    checkBufferCount(r1->bufferCount);
    checkBufferCount(r2->bufferCount);
    checkBufferCount(r3->bufferCount);

    return readyForHandling1 && readyForHandling2 && readyForHandling3;
}


void checkBufferCount(short buffCount) {
    if (buffCount == SENSOR_BUFFER_SIZE) {
        die("Panic! Buffer for sensor 1 data is full.");
    }
}

void cleanBuffer(SensorReading *reading, short sensor) {
    unsigned short tagDist;
    if (sensor == SENSOR_1)
        tagDist = ult1_TagDist;
    else if (sensor == SENSOR_2)
        tagDist = ult2_TagDist;
    else if (sensor == SENSOR_3)
        tagDist = ult3_TagDist;

    for (int i = 0; i < reading->bufferCount; i++) {
        if (reading->sensorReadingBuffer[i] > tagDist) {
            reading->bufferCount = reading->bufferCount - 1;
            for (int j = i; j < reading->bufferCount; j++) {
                reading->sensorReadingBuffer[j] = reading->sensorReadingBuffer[j + 1];
            }
            i--;
        }
    }
}

void handlePackage(PackageCollection *packages, SensorReading *r1, SensorReading *r2, SensorReading *r3) {
    serialDebugLN("Handling new package.");

    //cleaning buffer from obvious mistakes
    cleanBuffer(r1, SENSOR_1);
    cleanBuffer(r2, SENSOR_2);
    cleanBuffer(r3, SENSOR_3);

    // Create new package
    Package *package = &(packages->items[packages->count]);
    packages->count = packages->count + 1;
    package->id = __nextPackageId;
    __nextPackageId = __nextPackageId + 1;

    resetPackage(package);

    serialDebug("New package ID: ");
    serialDebugLN(String(package->id));

    //Fill Package object using collected sensor data
    handleSensorReadings(package, r1, r2, r3);

    //resetting sensorReadings
    resetSensorReading(r1);
    resetSensorReading(r2);
    resetSensorReading(r3);

}


void runScheduler() {
    // Setup
    serialDebug("Schedule started.\n");
    PackageCollection packages;
    SensorReading sensor1, sensor2, sensor3;
    
    // Initialisation
    resetSensorData(&sensor1, &sensor2, &sensor3);

    while (true) {
        bool readyToHandle = readSensorsEx(&sensor1, &sensor1, &sensor3);
        
        if (readyToHandle) {
            handlePackage(&packages, &sensor1, &sensor1, &sensor3);
        }
        
        receiveData(&packages);

        sendData(&packages);

        pushArm(&packages);
    }
}