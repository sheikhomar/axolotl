// LEDController.h

#ifndef _LEDCONTROLLER_h
#define _LEDCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Defines
#include "Defines.h"
#define STATUS_TIME_CYCLES 100
#define CYCLE_HALFTIME_CYCLES 25
#define ERROR_TIME_CYCLES 100

//Struct
typedef struct LedState {
	unsigned long statusCount = 0;
	unsigned long cycleCount = 0;
	unsigned long errorCount = 0;
	unsigned long statusLED = 0;
	unsigned long cycleLED = 0; 
	unsigned long errorLED = 0;
	unsigned int statusLEDTime = STATUS_TIME_CYCLES;
	unsigned int cycleLEDHalfTime = CYCLE_HALFTIME_CYCLES;
	unsigned int errorLEDTime = ERROR_TIME_CYCLES;
} LedState;

//Functions
void ledSetup(unsigned long ledA, unsigned long ledB, unsigned long ledC);
void led(unsigned long LED, bool TurnOn);
void updateLED(LedState *state);
void statusLamp(LedState *state);
void errorLamp(LedState *state);
void initLEDController(LedState *state, unsigned long statusLED, unsigned long cycleLED, unsigned long errorLED,
	unsigned long statusLEDTime = STATUS_TIME_CYCLES, unsigned long cycleLEDHalfTime = CYCLE_HALFTIME_CYCLES,
	unsigned long errorLEDTime = ERROR_TIME_CYCLES);

#endif

