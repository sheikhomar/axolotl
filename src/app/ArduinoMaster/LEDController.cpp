// 
// 
// 

#include "LEDController.h"

void ledSetup(long ledA, long ledB, long ledC) {
	pinMode(ledA, OUTPUT);
	pinMode(ledB, OUTPUT);
	pinMode(ledC, OUTPUT);

	digitalWrite(ledA, LOW);
	digitalWrite(ledB, LOW);
	digitalWrite(ledC, LOW);
}

void led(int LED, bool TurnOn) {
	digitalWrite(LED, TurnOn);
}