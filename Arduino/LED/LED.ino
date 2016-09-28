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

//Forward declaration
void TestLEDs();

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
  
  //Set default values of pins
    //LEDs
    digitalWrite(ult1_LED,LOW);
    digitalWrite(ult2_LED,LOW);
    digitalWrite(ult3_LED,LOW);
  
  delayMicroseconds(2);  
}
 


/***************************
loop

Main loop of the Arduino.
***************************/
void loop() {
  TestLEDs();
}


/***************************
PrintDistance

Prints the distance on the serial port and turns on/off the led based on tag
***************************/
void TestLEDs(){
  digitalWrite(ult1_LED,LOW);
  digitalWrite(ult2_LED,LOW);
  digitalWrite(ult3_LED,LOW);
  
  Serial.println("LED 1");
  digitalWrite(ult1_LED,HIGH);
  delay(1000);
  digitalWrite(ult1_LED,LOW);

  Serial.println("LED 2");
  digitalWrite(ult2_LED,HIGH);
  delay(1000);
  digitalWrite(ult2_LED,LOW);

  Serial.println("LED 3");
  digitalWrite(ult3_LED,HIGH);
  delay(1000);

  Serial.println("All OFF");
  digitalWrite(ult1_LED,LOW);
  digitalWrite(ult2_LED,LOW);
  digitalWrite(ult3_LED,LOW);
  delay(1000);

  Serial.println("All ON");
  digitalWrite(ult1_LED,HIGH);
  digitalWrite(ult2_LED,HIGH);
  digitalWrite(ult3_LED,HIGH);
  delay(1000);

  Serial.println("All OFF");
  digitalWrite(ult1_LED,LOW);
  digitalWrite(ult2_LED,LOW);
  digitalWrite(ult3_LED,LOW);
  delay(1000);
}
