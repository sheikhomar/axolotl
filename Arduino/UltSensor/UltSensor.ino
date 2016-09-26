/*
 * Test doc of Ult sensor
 * 
 * Manual link: http://www.micropik.com/PDF/HCSR04.pdf
*/

//Pins 
  //LEDs
  #define ult1_LED 11
  #define ult2_LED 10
  #define ult3_LED 9
  //Ult Sensors
  #define ult1_TrigPin 13
  #define ult1_echoPin 12
  #define ult2_TrigPin 8
  #define ult2_echoPin 7
  #define ult3_TrigPin 4
  #define ult3_echoPin 2
  //

//Values
#define ultMmPerUS 0.170145 //based on speed of sound * 0.5
#define ultMeasurementCycle 60 //60 ms, from manual
#define ultTrigPulse 10 //uS, from manual

//Forward declaration
void PrintData(bool Tag, double distance, double duration);
double GetUltDistance(int trigPin, int echoPin, bool delay);

/***************************
setup

Initiate all pins on the Arduino.
***************************/
void setup() {
  //Initiate Serial Communication
  Serial.begin(9600);
  
  //Initiate Pins
    //LEDs
    pinMode(ult1_LED, OUTPUT);
    pinMode(ult2_LED, OUTPUT);
    pinMode(ult3_LED, OUTPUT);
    
    //Ult Sensors
    pinMode(ult1_TrigPin, OUTPUT);
    pinMode(ult1_echoPin,INPUT);
    pinMode(ult2_TrigPin, OUTPUT);
    pinMode(ult2_echoPin,INPUT);
    pinMode(ult3_TrigPin, OUTPUT);
    pinMode(ult3_echoPin,INPUT);
  //Set default values of pins
    //LEDs
    digitalWrite(ult1_LED,LOW);
    digitalWrite(ult2_LED,LOW);
    digitalWrite(ult3_LED,LOW);
  
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
  Serial.println("Beginning UltraSound Sensor Test v. 1.0");
  
  Serial.println("Sensor 1");
  distance = GetUltDistance(ult1_TrigPin,ult1_echoPin,false);
  TestUltSensorsPrint(distance, distance < 60, ult1_LED);
  
  Serial.println("Sensor 2");
  distance = GetUltDistance(ult2_TrigPin,ult2_echoPin,false);
  TestUltSensorsPrint(distance, distance < 60, ult2_LED);
  
  Serial.println("Sensor 3");
  distance = GetUltDistance(ult3_TrigPin,ult3_echoPin,false);
  TestUltSensorsPrint(distance, distance < 60, ult3_LED);
  
  delay(ultMeasurementCycle);
}

/***************************
PrintDistance

Prints the distance on the serial port and turns on/off the led based on tag
***************************/
void TestUltSensorsPrint(double distance, bool tag, int LED){
  Serial.print(distance);
  Serial.print(" mm ");
  
  if(tag){
    Serial.println("| Tag");
    digitalWrite(LED,HIGH);
  }
  else{
  Serial.println("");
  digitalWrite(LED,LOW);
  }
}
