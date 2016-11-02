// 
// 
// 
#include "LEDController.h"
#include "SerialController.h"

#include "LEDControllerTest.h"


void ledBlinkTest() {
	const int time = 500;

	ledBlinkTestHelperFunction(LED_RED, time, "Red");
	ledBlinkTestHelperFunction(LED_GREEN, time, "Green");
	ledBlinkTestHelperFunction(LED_YELLOW, time, "YELLOW");
	ledBlinkTestHelperFunction(LED_BUILTIN, time, "BUILTIN");
}

void ledBlinkTestHelperFunction(long ledPin, long time, String message) {
	led(ledPin, true);
	serialDebugLN(message);
	delay(time);
	led(ledPin, false);
}