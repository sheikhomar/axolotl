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

    package->bin = BIN_REQUESTED;
}

void resendPackageInfoToRaspberryPI(Package *package) {
	serialSendData(RaspberryPi,'R');
	package->bin = BIN_REQUESTED_AGAIN;
	serialDebugLN("\nResending package to PI");
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
        
        Package *package = &(packages->items[i]);

        if (package->bin != BIN_NOT_REQUESTED && package->bin != BIN_REQUESTED && package->bin != BIN_REQUESTED_AGAIN) {
            unsigned long currentTime = millis();
            unsigned long timeDiff = currentTime - package->middleTime;

            if (package->bin == 1 && timeDiff >= FROM_ULT_TO_ARM1_MS) {
                serialSendData(NXT, COMM_NXT_PUSH_ARM1);
                removePackage(packages, i);
                i--;
            }
            else if (package->bin == 2 && timeDiff >= FROM_ULT_TO_ARM2_MS) {
                serialSendData(NXT, COMM_NXT_PUSH_ARM2);
                removePackage(packages, i);
                i--;
            } else if (package->bin != 1 && package->bin != 2) {
                serialDebug("Wrong bin for PID: ");
                serialDebugLN(String(package->id));
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

            package->colour = COLOUR_REQUESTED;
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
    if (packages->count <= 0 || packages->count > PACKAGE_BUFFER_SIZE) {
		String masterString = "Cannot remove package! - size ";
		masterString.concat(packages->count);

        die(masterString);
    }

	serialDebugLN("");
    serialDebug("Removing package: ");
    serialDebugLN(String(packages->items[index].id));

    // Remove package from the buffer
    for (int i = index; i < packages->count; i++) {
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


    if (clientInfo != Arduino) {
        return;
    }

	if (packages->count == 0) {
		return;
	}

    Package *package = &(packages->items[0]);
     
    if (command == COMM_ARDUINO_COLOUR_INFO) {
        // We have recieved data from NXT
        package->colour = buf[0];
        serialDebug("\nReceived Colour: [");
        serialDebug(String(package->colour));
        serialDebug("] for PID: ");
        serialDebugLN(String(package->id));

        if (package->colour == COLOUR_BLACK || package->colour == COLOUR_UNKNOWN) {
            // If we get these colours then, we just remove the package
            serialDebugLN("\nBad colour");

            // Remove current package.
            removePackage(packages, 0);
        }
    }
    else if (command == COMM_PI_ADVICEPACKAGE) {
        // We have recieved data from Raspberry Pi
        package->bin = buf[0];
        serialDebug("\nReceived bin: [");
        serialDebug(String(package->bin));
        serialDebug("] for PID: ");
        serialDebugLN(String(package->id));
    }
    else {
        serialDebug("\nWrong command: ");
        serialDebugLN(String(command));
        
        // Wrong command. Remove package.
        removePackage(packages, 0);
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
    //serialDebug("Number of packages: ");
    //serialDebugLN(String(packages->count));
	bool hasRequestedPacking = false; //PI can only handle one request at a time.

    for (int i = 0; i < packages->count; i++) {
        Package *package = &(packages->items[i]);


        //Request Colour
        if (package->colour == COLOUR_NOT_REQUESTED) {
            requestColourInformation(package);
        }
        
		//Request Packing
        if (!hasRequestedPacking &&
			package->colour != COLOUR_REQUESTED &&
            package->colour != COLOUR_NOT_REQUESTED &&
			(package->bin == BIN_NOT_REQUESTED || package->bin == BIN_REQUESTED || package->bin == BIN_REQUESTED_AGAIN)) {
				
			hasRequestedPacking = true;

			if (package->bin == BIN_NOT_REQUESTED) {
				sendPackageInfoToRaspberryPi(package);
				packages->packageTimeoutMS = millis() + PI_RESPONSE_TIMEOUT_MS;
			}
			else if (packages->packageTimeoutMS < millis()) {
				if (package->bin == BIN_REQUESTED) {
					resendPackageInfoToRaspberryPI(package);
					packages->packageTimeoutMS = millis() + PI_RESPONSE_TIMEOUT_MS;
				}
				else {
					removePackage(packages, i);
					serialDebugLN("Removed timedout bin package");
					i--;
				}
			}
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

    // checking buffer bound
    checkBufferCount(r1->bufferCount, r2->bufferCount, r3->bufferCount);
	
	//Reading sensors
    delay(5);
    bool readyForHandling2 = readSensor(r2, SENSOR_2);
    delay(5);
    bool readyForHandling1 = readSensor(r1, SENSOR_1);
    delay(5);
    bool readyForHandling3 = readSensor(r3, SENSOR_3);

	//Handle sensor noise
	handleSensorNoise(r1);
	handleSensorNoise(r2);
	handleSensorNoise(r3);

	return readyForHandling1 && readyForHandling2 && readyForHandling3;
}

void handleSensorNoise(SensorReading *reading) {
	if (reading->falseCount >= NOT_DETECTED_THRESHOLD && 
		reading->bufferCount <= (NOT_DETECTED_THRESHOLD + SENSOR_ALLOWED_FALSE_POSITIVES)) {
		resetSensorReading(reading);
	}
}


void checkBufferCount(unsigned short buffer1,unsigned short buffer2, unsigned short buffer3) {
    if (buffer1 >= SENSOR_BUFFER_SIZE ||
		buffer2 >= SENSOR_BUFFER_SIZE ||
		buffer3 >= SENSOR_BUFFER_SIZE) {
		serialDebugLN("");
		serialDebug("Bufff 1 ");
		serialDebugLN(String(buffer1));
		serialDebug("Buff 2 ");
		serialDebugLN(String(buffer2));
		serialDebug("Buff 3 ");
		serialDebugLN(String(buffer3));

        die("Panic! Buffer for sensor data is full.");
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
    //Cleaning buffer from obvious mistakes
    cleanBuffer(r1, SENSOR_1);
    cleanBuffer(r2, SENSOR_2);
    cleanBuffer(r3, SENSOR_3);

	//Check bound for packages
	if (packages->count >= PACKAGE_BUFFER_SIZE) {
		String masterString = "Reached max packages _ ";
		masterString.concat(packages->count);
		die(masterString);
	}


    // Create new package
    Package *package = &(packages->items[packages->count]);
    packages->count = packages->count + 1;
    resetPackage(package);
    (*package).id = __nextPackageId;
    __nextPackageId = __nextPackageId + 1;

    

    serialDebug("\nNew package with ID: ");
    serialDebugLN(String(package->id));

    //Fill Package object using collected sensor data
    handleSensorReadings(package, r1, r2, r3);

    //resetting sensorReadings
    resetSensorReading(r1);
    resetSensorReading(r2);
    resetSensorReading(r3);

}

void packageEmulator(PackageCollection *packages, int *count) {
    *count += 1;
    if (*count == 400) {
        *count = 0;
        serialDebugLN("Creating new package.");


		if (packages->count >= PACKAGE_BUFFER_SIZE) {
			String masterString = "Reached max packages _ ";
			masterString.concat(packages->count);
			die(masterString);
		}


        Package *package = &(packages->items[packages->count]);
        packages->count += 1;
        resetPackage(package);

        package->id = __nextPackageId;
        __nextPackageId += 1;
        
        package->height = 2000;
        package->width = 3200;
        package->length = 1600;
        package->middleTime = 400;

        printPackages(packages);
    }
    delay(10);
}

void printPackages(PackageCollection *packages) {
	serialDebugLN("");
	serialDebug("Count ");
	serialDebug(String(packages->count));
	serialDebug(" Timeout ");
	serialDebug(String(packages->packageTimeoutMS));
	serialDebugLN("");

    for (int i = 0; i < PACKAGE_BUFFER_SIZE; i++) {
        Package *package = &(packages->items[i]);
		serialDebug("");
		serialDebug("[");
		serialDebug(String(i));
		serialDebug("] ");
		serialDebug(String(package->id));
		serialDebug(" ");
		serialDebug(String(package->length));
		serialDebug(" ");
		serialDebug(String(package->colour));
		serialDebug(" ");
		serialDebugLN(String(package->bin));
    }
}


void runScheduler() {
    // Setup
    serialDebug("Scheduler started.\n");
    PackageCollection packages;
    SensorReading sensor1, sensor2, sensor3;
    

    // Initialisation
    resetSensorData(&sensor1, &sensor2, &sensor3);
    int count = 0;

    while (true) {
        bool readyToHandle = readSensorsEx(&sensor1, &sensor2, &sensor3);

        if (readyToHandle) {
            handlePackage(&packages, &sensor1, &sensor2, &sensor3);
        }
        
        receiveData(&packages);

        sendData(&packages);

        pushArm(&packages);
    }
}