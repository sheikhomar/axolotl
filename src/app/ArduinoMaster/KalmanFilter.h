// KalmanFilter.h

#ifndef _KALMANFILTER_h
#define _KALMANFILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Defines.h"

void kalmanTestFunction();
void calculateKalmanGain(KalmanFilterInformation *kalmanFilterInfo);
void calculateKalmanEstimate(KalmanFilterInformation *kalmanFilterInfo, short measurement);
void calculateKalmanErrorInEstimate(KalmanFilterInformation *kalmanFilterInfo);
void resetKalmanFilter(KalmanFilterInformation *kalmanFilterInfo);
void updatePrediction(KalmanFilterInformation *kalmanFilterInfo);

#endif

