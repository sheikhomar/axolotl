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

void readColourInfo(Package *package) {
  byte buf[] = { 0 };

  // Request colour information from the NXT
  serialSendData(NXT, buf, 0, 5);

  // Block until data is recieved from the NXT.
  while (serialCheck() != Arduino);

  serialReadData(buf, 1);
  package->colour = buf[0];
}

void sendPackageInfoToRaspberryPi(Package *package) {
  byte buf[] = { (byte)package->width, (byte)package->length, (byte)package->height, (byte)package->colour };
  serialSendData(RaspberryPi, buf, 4, 1);
}

void readPackingAdviceIfAny(Package packages[], int startIndex, int numberOfPackages) {
  byte buf[1];
  serialReadData(buf, 1);
  // TODO: Ensure that the received data is correct before we continue.
}

void handlePackages(Package packages[], int startIndex, int numberOfPackages) {
    int index = 0;
    for (int i = startIndex; i < startIndex + numberOfPackages; i++) {
        index = i % PACKAGE_BUFFER_SIZE;
        Package p = packages[index];
        if (p.colour == COLOUR_NONE) {
            // Calculate when we can read the colour sensor
            unsigned long currentTime = millis();

            if (currentTime - p.middleTime >= FROM_ULT_TO_COLOUR_SENSOR_MS) {
                readColourInfo(&p);
                sendPackageInfoToRaspberryPi(&p);
            }
        }
    }
}

void resetPackage(Package *package) {
  package->length = 0;
  package->width = 0;
  package->height = 0;
  package->colour =  COLOUR_NONE;
  package->middleTime = 0;
  package->bin = -1;
}

void resetPackages(Package packages[]) {
  for (int i = 0; i < PACKAGE_BUFFER_SIZE; i++) {
    resetPackage(&packages[i]);
  }
}

void runScheduler() {
    Package packages[PACKAGE_BUFFER_SIZE];
    int packageStartIndex = 0;
    int packageCount = 0;

    SensorData sensorBuffer[SENSOR_BUFFER_SIZE];
    int sensorBufferStartIndex = 0;
    int sensorBufferCount = 0;

    resetPackages(packages);

    while (true) {
        SensorData *sd = &sensorBuffer[(sensorBufferStartIndex + sensorBufferCount) % SENSOR_BUFFER_SIZE];
        bool newPackageDetected = readSensors(sd);
        if (newPackageDetected) {
            // We have detected a new package in the conveyor belt

            if (sensorBufferCount == SENSOR_BUFFER_SIZE) {
                die("Panic! Buffer for sensor data is full.");
            }
            
            sensorBufferCount++;
        }
        else {
            if (sensorBufferCount > 0) {
                // At this stage, we have collected distance information for a single package.
                // The function 'handleSensorData' builds an instance of Package based 
                // on the data in the sensorData.

                if (packageCount == PACKAGE_BUFFER_SIZE) {
                    die("Panic! Buffer for packages is full.");
                }
                
                // Find current package
                Package *p = &packages[(packageStartIndex + packageCount) % PACKAGE_BUFFER_SIZE];

                // Prepare next package
                packageCount++;

                // Fill Package object using collected sensor data
                handleSensorData(p, sensorBuffer, sensorBufferStartIndex, sensorBufferCount);

                //serialDebug("Package: " + String(p.width) + " x " + String(p.height) + " x " + String(p.length) + "\n");

                // Empty buffer for sensor data
                sensorBufferCount = 0;
            }
        }

        if (packageCount > 0) {
          handlePackages(packages, packageStartIndex, packageCount);
          readPackingAdviceIfAny(packages, packageStartIndex, packageCount);
        }
    }
}
