/*
 * Test doc of Ult sensor
 * 
 * Manual link: http://www.micropik.com/PDF/HCSR04.pdf
*/

//Pins 
  //Ult Sensors
  #define ult1_TrigPin 2
  #define ult1_echoPin 3
  #define ult2_TrigPin 4
  #define ult2_echoPin 5
  #define ult3_TrigPin 6
  #define ult3_echoPin 7
  //

//Values
#define ultMmPerUS 0.170145 //based on speed of sound * 0.5
#define ultMeasurementCycle 60 //60 ms, from manual
#define ultTrigPulse 10 //uS, from manual

//Forward declaration
void PrintData(bool Tag, double distance, double duration);
double GetUltDistance(int trigPin, int echoPin, bool delay);
void TestUltSensorsPrint(double distance, bool tag);

/***************************
setup

Initiate all pins on the Arduino.
***************************/
void setup() {
  //Initiate Serial Communication
  Serial.begin(57600);
  
  //Initiate Pins
    //Ult Sensors
    pinMode(ult1_TrigPin, OUTPUT);
    pinMode(ult1_echoPin,INPUT);
    pinMode(ult2_TrigPin, OUTPUT);
    pinMode(ult2_echoPin,INPUT);
    pinMode(ult3_TrigPin, OUTPUT);
    pinMode(ult3_echoPin,INPUT);
  //Set default values of pins
    //Ult Sensors
    digitalWrite(ult1_TrigPin, LOW);
    digitalWrite(ult2_TrigPin, LOW);
    digitalWrite(ult2_TrigPin, LOW);
  
  delayMicroseconds(2);  
}
 

/***************************
loop

Main loop of the Arduino.
***************************/
void loop() {
  TestUltSensors();
}

/***************************
GetDistance

Get the distance from a specified ultraSoundSensor via its triggerPin and echoPin.
The delay can be used to avoid polling issues should the same sensor be called multiple times in a row.
***************************/
double GetUltDistance(int trigPin, int echoPin, bool wait){
  double duration, distance;

  if(wait)
    delay(ultMeasurementCycle);
  
  //TriggerPulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(ultTrigPulse);
  digitalWrite(trigPin, LOW);

  //Receive echo signal
  duration = pulseIn(echoPin, HIGH); //reads the distance as time of a pulse
  
  //Return distance
  return duration*ultMmPerUS; 
}



/***************************
TestUltSensors

Prints the measured distance of all UltSensors and turns on their LEDs in acc.
to its idle distance.
***************************/
void TestUltSensors(){
  double distance;

  Serial.print("Sensor 1 |");
  distance = GetUltDistance(ult1_TrigPin,ult1_echoPin,false);
  TestUltSensorsPrint(distance, distance < 45);
    
  Serial.print("Sensor 2 |");
  distance = GetUltDistance(ult2_TrigPin,ult2_echoPin,false);
  TestUltSensorsPrint(distance, distance < 60);

  Serial.print("Sensor 3 |");
  distance = GetUltDistance(ult3_TrigPin,ult3_echoPin,false);
  TestUltSensorsPrint(distance, distance < 33);

  delay(ultMeasurementCycle);
}

/***************************
PrintDistance

Prints the distance on the serial port and turns on/off the led based on tag
***************************/
void TestUltSensorsPrint(double distance, bool tag){
  Serial.print(distance);
  Serial.print(" mm ");
  
  if(tag){
    Serial.println("| Tag");
  }
  else{
  Serial.println("");
  }
}
