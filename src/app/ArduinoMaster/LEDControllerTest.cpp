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
}

void ledBlinkTestHelperFunction(long ledPin, long time, String message) {
	led(ledPin, true);
	serialDebugLN(message);
	delay(time);
	led(ledPin, false);
}

void ledCycleTest() {
	int count = 0;
	LedState ledState;

	initLEDController(&ledState, LED_GREEN, LED_YELLOW, LED_RED, 100,25,100);
	
	while (true)
	{
		++count %= 900000;

		if ((count % 300) == 0) {
			statusLamp(&ledState);
		}

		if ((count % 500) == 0) {
			errorLamp(&ledState);
		}

		updateLED(&ledState);
		delay(10);
	}
}