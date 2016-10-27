void determineTimeBetweenSensors() {
  unsigned long iniTime = 0, timeColour = 0, travelTime = 0;
  bool sensorRead = false;
  byte data2[1] = {50};

  serialSendData(NXT, data2, 1, 6); //Set speed
  serialSendData(NXT, data2, 1, 3); //Start conveyor belt

  while (!sensorRead) { //Check when ult sensor register object
    sensorRead = readSensorsBest();
   //switch(sensorRead){
    //case false: serialDebug("false"); break;
    //case true: serialDebug("true"); break;
    //default: break;
   //}
   delay(ULT_MEASUREMENT_CYCLE_MS);
  }

  iniTime = millis();
  readColourSensor(); //Reading colour sensor -- returning from func when a brick colour is read

  timeColour = millis();
  travelTime = timeColour - iniTime; //The time from when ult sensor register object untill colour sensor do
  serialDebug("Time: ");
  serialDebug((String)travelTime);

  //serialSendData(NXT, data2, 0, 4);
  //while(1);
  return;
}

void readColourSensor() {
  bool ready = false;
  byte dataS[0];
  byte dataRec[1];
  byte colour = -1;
  client sender = unknown;

  serialSendData(NXT, dataS, 0, 11);
  do {
     sender = serialReadData(dataRec, 1);
  } while (sender != Arduino);
  colour = dataRec[0];
  serialDebug(determineColour(colour));
}

bool readSensorsBest() { //Returning true when an object is detected by ult sensors
  bool sensor1, sensor2, sensor3;
  unsigned short dist1, dist2, dist3;

  dist1 = GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
  sensor1 = dist1 < ult1_TagDist;

  dist2 = GetUltDistance(ULT2_TRIG_PIN, ULT2_ECHO_PIN, false);
  sensor2 = dist2 < ult2_TagDist;

  dist3 = GetUltDistance(ULT3_TRIG_PIN, ULT3_ECHO_PIN, false);
  sensor3 = dist3 < ult3_TagDist;

  return sensor1 && sensor2;
}

String determineColour(byte input){
     switch(input) {
      case 0: return "Red";
      case 1: return "Green";
      case 2: return "Blue"; 
      case 3: return "Yellow";
      default: return "Irrelevant";
    }
  }

