// 
// 
// 
#include "Defines.h"

#include "SlidingWindowFilter.h"

void initSlidingWindowInformation(SlidingWindowInformation *swi) {
	swi->count = 0;
	swi->currentValue = 0;
	swi->sumOfMeasurements = 0;
	
	for (int i = 0; i < SLIDING_WINDOW_K; i++) {
		swi->measurements[i] = 0;
	}
}

void updateSlidingWindowFilter(SlidingWindowInformation *swi, double measurement) {
	swi->sumOfMeasurements = swi->sumOfMeasurements - swi->measurements[swi->count % SLIDING_WINDOW_K];
	swi->measurements[swi->count % SLIDING_WINDOW_K] = measurement;
	swi->sumOfMeasurements = swi->sumOfMeasurements + measurement;

	swi->count = swi->count + 1; 

	if (swi->count >= SLIDING_WINDOW_K) {
		swi->currentValue = swi->sumOfMeasurements / SLIDING_WINDOW_K;
	}
	else {
		swi->currentValue = 0;
	}
}
