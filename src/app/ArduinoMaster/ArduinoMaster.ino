// the setup function runs once when you press reset or power the board
#include "Defines.h"
#include "UltrasoundSensorTest.h"
#include "SerialControllerTest.h"
#include "PositionTest.h"
#include "Utilities.h"
#include "UltrasoundSensor.h"
#include "SerialController.h"
#include "Scheduler.h"
#include "ObjectIdentification.h"
#include "LEDController.h"
void setup() {

	//Initiate Serial Communication
	Serial.begin(BAUD_DEBUG);
	RS485Serial.begin(BAUD);

	//Initiate Pins
	//LEDs
	pinMode(LED1_PIN, OUTPUT);
	pinMode(LED2_PIN, OUTPUT);
	pinMode(LED3_PIN, OUTPUT);
	//Ult Sensors
	pinMode(ULT1_TRIG_PIN, OUTPUT);
	pinMode(ULT1_ECHO_PIN, INPUT);
	pinMode(ULT2_TRIG_PIN, OUTPUT);
	pinMode(ULT2_ECHO_PIN, INPUT);
	pinMode(ULT3_TRIG_PIN, OUTPUT);
	pinMode(ULT3_ECHO_PIN, INPUT);
	//RS485
	pinMode(SERIAL_TRANSMIT_PIN, OUTPUT);

	//Set default values of pins
	//LEDs
	digitalWrite(LED1_PIN, LOW);
	digitalWrite(LED2_PIN, LOW);
	digitalWrite(LED3_PIN, LOW);
	//Ult Sensors
	digitalWrite(ULT1_TRIG_PIN, LOW);
	digitalWrite(ULT2_TRIG_PIN, LOW);
	digitalWrite(ULT2_TRIG_PIN, LOW);
	//RS485
	digitalWrite(SERIAL_TRANSMIT_PIN, LOW);

	delayMicroseconds(20);
	serialDebugLN("Starting Arduino");
}

// the loop function runs over and over again until power down or reset
void loop() {
	
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);

}
