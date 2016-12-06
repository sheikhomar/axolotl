// 
// 
// 

#include "LEDController.h"

void ledSetup(unsigned long ledA, unsigned long ledB, unsigned long ledC) {
	pinMode(ledA, OUTPUT);
	pinMode(ledB, OUTPUT);
	pinMode(ledC, OUTPUT);

	digitalWrite(ledA, LOW);
	digitalWrite(ledB, LOW);
	digitalWrite(ledC, LOW);
}


void led(unsigned long LED, bool TurnOn) {
	digitalWrite(LED, TurnOn);
}


void initLEDController(LedState *state, unsigned long statusLED, unsigned long cycleLED, unsigned long errorLED,
						unsigned long statusLEDTime = STATUS_TIME_CYCLES, unsigned long cycleLEDHalfTime = CYCLE_HALFTIME_CYCLES, 
						unsigned long errorLEDTime = ERROR_TIME_CYCLES) {

	state->statusLED = statusLED;
	state->cycleLED = cycleLED;
	state->errorLED = errorLED;
	state->statusLEDTime = statusLEDTime;
	state->cycleLEDHalfTime = cycleLEDHalfTime;
	state->errorLEDTime = errorLEDTime;

}

void updateLED(LedState *state) {
	unsigned long halfCycle = state->cycleLEDHalfTime;
	unsigned long fullCycle = halfCycle * 2;

	//Update cycle count
	state->cycleCount = ((state->cycleCount) % fullCycle) + 1;

	//Update cycle timer
	if (state->cycleCount == halfCycle) {
		led(state->cycleLED, true);
	}
	else if (state->cycleCount == fullCycle) {
		led(state->cycleLED, false);
	}

	//Count status lamp
	if (state->statusCount > 0) {
		state->statusCount -= 1;

		if (state->statusCount == 0) {
			led(state->statusLED, false);
		}
	}

	//Count debug lamp
	if (state->errorCount > 0) {
		state->errorCount -= 1;

		if (state->errorCount == 0) {
			led(state->errorLED, false);
		}
	}


}

void statusLamp(LedState *state) {
	state->statusCount = state->statusLEDTime;
	led(state->statusLED, true);
}

void errorLamp(LedState *state) {
	state->errorCount = state->errorLEDTime;
	led(state->errorLED, true);
}
