void determineTimeBetweenSensors() {
  unsigned long iniTime = 0, timeColour = 0, travelTime = 0;
  bool sensorRead = false;
  byte data2[0];
  
  serialSendData(NXT, data2, 0, 3);
serialDebug("Her ");
  while (!sensorRead) {
    sensorRead = readSensorsBest();
   //switch(sensorRead){
    //case false: serialDebug("false"); break;
    //case true: serialDebug("true"); break;
    //default: break;
   //}
   delay(ULT_MEASUREMENT_CYCLE_MS);
  }
serialDebug("Her1");
  iniTime = millis();
  sensorRead = false;
  while (!sensorRead) {
    sensorRead = colourSensorReady();
  }
serialDebug("Her3");
  timeColour = millis();
  travelTime = timeColour - iniTime;
  serialDebug("Time: ");
  serialDebug((String)travelTime);
  return;
}

bool colourSensorReady() {
  bool ready = false;
  byte dataS[] = {5};
  byte dataRec[1];
  byte colour = -1;
  client sender = unknown;

  while (!(colour == 0 || colour == 1 || colour == 2 || colour == 3)) {
    serialSendData(NXT, dataS, 1, 5);
    do {
       sender = serialReadData(dataRec, 1);
       delay(10);
    } while (sender != Arduino);
    colour = dataRec[0];
    //delay(500);
  }
  serialDebug(determineColour(colour));
  return true;
}

bool readSensorsBest() {
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

