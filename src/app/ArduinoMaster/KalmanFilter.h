// KalmanFilter.h

#ifndef _KALMANFILTER_h
#define _KALMANFILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Defines.h"

void runKalmanScheduler();
void calculateKalmanGain(KalmanFilterInformation *kalmanFilterInfo, short measurement);
void calculateKalmanEstimate(KalmanFilterInformation *kalmanFilterInfo, short measurement);
void calculateKalmanErrorInEstimate(KalmanFilterInformation *kalmanFilterInfo);

#endif

