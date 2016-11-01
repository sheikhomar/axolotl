// 
// 
// 

#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"
#include "ObjectIdentification.h"

#include "LEDController.h"

void LED(int LED, bool TurnOn) {
	digitalWrite(LED, TurnOn);
}