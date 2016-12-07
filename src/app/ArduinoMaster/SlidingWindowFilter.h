// SlidingWindowFilter.h

#ifndef _SLIDINGWINDOWFILTER_h
#define _SLIDINGWINDOWFILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#endif
#include "Defines.h"

void initSlidingWindowInformation(SlidingWindowInformation *swi);
void updateSlidingWindowFilter(SlidingWindowInformation *swi, double measurement);


