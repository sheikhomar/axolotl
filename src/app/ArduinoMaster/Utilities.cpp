// 
// 
// 

#include "Defines.h"
#include "SerialController.h"
#include "LEDController.h"

#include "Utilities.h"


void die(String abortMessage) {
	serialDebug(abortMessage);
	while (true) {
		led(LED_BUILTIN, true);
		led(LED_GREEN, true);
		led(LED_RED, false);
		led(LED_YELLOW, false);

		delay(1000);

		led(LED_BUILTIN, false);
		led(LED_GREEN, false);
		led(LED_RED, true);
		led(LED_YELLOW, true);

		delay(1000);
	}
}

void runConveyorBeltAtSpeed(byte speed) {
	byte data[1] = { speed };
	serialSendData(NXT, data, 1, COMM_NXT_ADJUST_BELT_SPEED);
	serialSendData(NXT, data, 0, COMM_NXT_START_BELT);
}
