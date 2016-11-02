// 
// 
// 

#include "LEDController.h"

void ledSetup(long ledA, long ledB, long ledC, long ledD) {
	pinMode(ledA, OUTPUT);
	pinMode(ledB, OUTPUT);
	pinMode(ledC, OUTPUT);
	pinMode(ledD, OUTPUT);

	digitalWrite(ledA, LOW);
	digitalWrite(ledB, LOW);
	digitalWrite(ledC, LOW);
	digitalWrite(ledD, LOW);
}

void led(int LED, bool TurnOn) {
	digitalWrite(LED, TurnOn);
}