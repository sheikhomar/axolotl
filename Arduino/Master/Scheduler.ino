#define COLOUR_NONE -1
#define COLOUR_RED 0
#define COLOUR_GREEN 1
#define COLOUR_BLUE 2
#define COLOUR_YELLOW 3
#define COLOUR_WHITE 6
#define COLOUR_BLACK 7
#define COLOUR_UNKNOWN 128

#define FROM_ULT_TO_COLOUR_SENSOR_MS 300
#define FROM_ULT_TO_ARM1_MS 300
#define FROM_ULT_TO_ARM2_MS 300

#define SENSOR_BUFFER_SIZE 100
#define PACKAGE_BUFFER_SIZE 10


void handlePackages(Package packages[], int bufferStartIndex, int bufferEndIndex) {
    for (int i = 0; i < PACKAGE_BUFFER_SIZE; i++) {
        int index = (i + bufferStartIndex) % PACKAGE_BUFFER_SIZE;
        // Do stuff with packages[index]
    }
}

void schedule() {
    Package packages[PACKAGE_BUFFER_SIZE];
    unsigned short packageStartIndex = 0;
    unsigned short packageEndIndex = 0;

    SensorData sensorBuffer[SENSOR_BUFFER_SIZE];
    unsigned short sensorBufferStartIndex = 0;
    unsigned short sensorBufferEndIndex = 0;

    while (true) {
        bool newPackageDetected = readSensors(sensorBuffer, sensorBufferEndIndex);
        if (newPackageDetected) {
            // We have detected a new package in the conveyor belt

            sensorBufferEndIndex++;
        }
        else {

            if (sensorBufferStartIndex != sensorBufferEndIndex) {
                // At this stage we have collected distance information for a single package.
                // The function 'handleSensorData' builds an instance of Package based 
                // on the data in the sensorData.
                handleSensorData(&packages[packageEndIndex],
                    sensorBuffer, sensorBufferStartIndex, sensorBufferEndIndex);

                //prepareNextPackage(packages);
                sensorBufferStartIndex = sensorBufferEndIndex;

                //Package p = packages[packageEndIndex];
                //String t2 = String(p.width);
                //serialDebug("Package: " + String(p.width) + " x " + String(p.height) + " x " + String(p.length) + "\n");
            }
        }

        handlePackages(packages, packageStartIndex, packageEndIndex);

    }
}