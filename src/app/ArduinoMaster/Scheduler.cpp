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