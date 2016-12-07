// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"
#include "PackageIdentification.h"
#include "LEDController.h"

#include "Scheduler.h"

// Global variables
unsigned int __nextPackageId = 1;
int debugLEDCounter = 0;
LedState ledState;

void sendPackageInfoToRaspberryPi(Package *package) {
    byte buf[] = { 0, 0, 0, 0 };

    buf[0] = convertMeasuredValueToMillimetres(package->width);
    buf[1] = convertMeasuredValueToMillimetres(package->length);
	buf[2] = convertMeasuredValueToMillimetres(package->height);
    buf[3] = package->colour;

    serialSendData(RaspberryPi, buf, 4, 'p');
    serialDebug("\n");

    package->bin = BIN_REQUESTED;
}

byte convertMeasuredValueToMillimetres(unsigned short number) {
    return number / 100;
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
                serialDebug("\n--->Wrong bin for PID: ");
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

        if (timeDiff >= (FROM_ULT_TO_COLOUR_SENSOR_MS + FROM_ULT_TO_COLOUR_SENSOR_MS_DELAY)) {
            serialDebug("Colour requested for PID: ");
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

    serialDebug("Removing package: ");
    serialDebugLN(String(packages->items[index].id));

    // Remove package from the buffer
    for (int i = index; i < packages->count; i++) {
        packages->items[i] = packages->items[i + 1];
    }

    packages->count = packages->count - 1;
}


Package *findNextPackageForColour(PackageCollection *packages) {
    for (int i = 0; i < packages->count; i++) {
        if (packages->items[i].colour == COLOUR_REQUESTED) {
            return &(packages->items[i]);
        }
    }
}

Package *findNextPackageForPlacement(PackageCollection *packages) {
    for (int i = 0; i < packages->count; i++) {
        if (packages->items[i].bin == BIN_REQUESTED || 
            packages->items[i].bin == BIN_REQUESTED_AGAIN) {
            return &(packages->items[i]);
        }
    }
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

short findNextColorRequested(PackageCollection *packages, short id) {
	short returnValue = 0;

	for (short i = 0; i < packages->count; i++) {
		if (packages->items[i].id == id) {
			returnValue = i;
		}
	}
	return returnValue;
}

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

    
     
    if (command == COMM_ARDUINO_COLOUR_INFO) {
        Package *package = findNextPackageForColour(packages);
        
        // We have recieved data from NXT
        package->colour = buf[0];
        serialDebug("\nReceived Colour: [");
        serialDebug(String(package->colour));
        serialDebug("] for PID: ");
        serialDebugLN(String(package->id));

		//Check the colour
		byte colour = byte(package->colour);
		bool correctColour = (colour == COLOUR_BLUE || colour == COLOUR_GREEN ||
			colour == COLOUR_RED || colour == COLOUR_YELLOW);

        if (package->colour == COLOUR_UNKNOWN || !correctColour) {
            // NXT was unable to find a colour, we'll request it again
			errorLamp(&ledState);
			if (COLOUR_UNKNOWN) {
				serialDebugLN("\n--->No colour");
			}
			else {
				serialDebugLN("\n--->Incorrect colour");
			}

			removePackage(packages, findNextColorRequested(packages, package->id));
        }
    }
    else if (command == COMM_PI_ADVICEPACKAGE) {
        Package *package = findNextPackageForPlacement(packages);
        // We have recieved data from Raspberry Pi
        package->bin = buf[0];
        serialDebug("\nReceived bin: [");
        serialDebug(String(package->bin));
        serialDebug("] for PID: ");
        serialDebugLN(String(package->id));
    }
    else {
		errorLamp(&ledState);
        serialDebug("\n--->Wrong command: ");
        serialDebugLN(String(command));
        
        // Wrong command. Die
		die("\nWrong command");
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
	bool hasRequestedColour = false; //NXT can only handle one request at a time.


    for (int i = 0; i < packages->count; i++) {
        Package *package = &(packages->items[i]);


        //Request Colour
        if (!hasRequestedColour &&
			(package->colour == COLOUR_NOT_REQUESTED || package->colour == COLOUR_REQUESTED)) {
			hasRequestedColour = true;

			if (package->colour == COLOUR_NOT_REQUESTED) {
				requestColourInformation(package);
				packages->colourTimeoutMS = millis() + NXT_REQUEST_TIMEOUT_MS;
			}
			else if (packages->colourTimeoutMS < millis()) {
				errorLamp(&ledState);
				serialDebugLN("\n--->Timedout colour request");
				removePackage(packages, i);
				i--;
			}
        }
        
		//Request Packing
        if (!hasRequestedPacking &&
			package->colour != COLOUR_REQUESTED &&
            package->colour != COLOUR_NOT_REQUESTED &&
			(package->bin == BIN_NOT_REQUESTED || package->bin == BIN_REQUESTED || package->bin == BIN_REQUESTED_AGAIN)) {
				
			hasRequestedPacking = true;

			if (package->bin == BIN_NOT_REQUESTED) {
				sendPackageInfoToRaspberryPi(package);
				packages->packageTimeoutMS = millis() + PI_REQUEST_TIMEOUT_MS;
			}
			else if (packages->packageTimeoutMS < millis()) {
				errorLamp(&ledState);
				serialDebugLN("\n--->Timedout bin package");
				removePackage(packages, i);
				i--;
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

void cleanBuffer(SensorReading *reading, short sensor) {
    unsigned short tagDist;
    if (sensor == ULT_TOP_SENSOR)
        tagDist = ULT_TOP_TAG_DIST;
    else if (sensor == ULT_RIGHT_SENSOR)
        tagDist = ULT_RIGHT_TAG_DIST;
    else if (sensor == ULT_LEFT_SENSOR)
        tagDist = ULT_LEFT_TAG_DIST;

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
    cleanBuffer(r1, ULT_TOP_SENSOR);
    cleanBuffer(r2, ULT_RIGHT_SENSOR);
    cleanBuffer(r3, ULT_LEFT_SENSOR);

	//Check bound for packages
	if (packages->count >= PACKAGE_BUFFER_SIZE) {
		serialDebug("Reached max packages _ ");
		serialDebugLN(String(packages->count));
		die("");
	}


    // Create new package
    Package *package = &(packages->items[packages->count]);
    packages->count = packages->count + 1;
    resetPackage(package);
    (*package).id = __nextPackageId;
    __nextPackageId = __nextPackageId + 1;

    

	serialDebug("\nNew package with ID: #");
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
    serialDebug("Scheduler v2 started.\n");
    PackageCollection packages;
    PackageIdentificationState state;

    // Initialisation
	initLEDController(&ledState, LED_GREEN,LED_YELLOW,LED_RED);
	initPackageIdentification(&state);

    while (true) {
        runIdentification(&state, &packages);
        receiveData(&packages);
        sendData(&packages);
        pushArm(&packages);
		updateLED(&ledState);
    }
}