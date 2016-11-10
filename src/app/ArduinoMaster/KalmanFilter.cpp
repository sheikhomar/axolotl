// 
// 
// 
#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "KalmanFilter.h"

void runKalmanScheduler() {
	KalmanFilterInformation kalmanFilterInfo;
	double measurement = 0;

	kalmanFilterInfo.kalmanGain = 0;
	kalmanFilterInfo.errorInEstimate = 1000;
	kalmanFilterInfo.previousErrorInEstimate = 0;
	kalmanFilterInfo.errorInMeasurement = 500;
	kalmanFilterInfo.currentEstimate = 5000;
	kalmanFilterInfo.previousEstimate = 0;

	while (true) {
		delay(5);
		measurement = (double)GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);
		calculateKalmanGain(&kalmanFilterInfo, measurement);
		calculateKalmanEstimate(&kalmanFilterInfo, measurement);
		calculateKalmanErrorInEstimate(&kalmanFilterInfo);
		serialDebug("Measured distance: " + String(measurement));
		serialDebug(" Kalman Gain: " + String(kalmanFilterInfo.kalmanGain));
		serialDebug(" Kalman Estimate: " + String(kalmanFilterInfo.currentEstimate));
		serialDebugLN(" Error in Estimate: " + String(kalmanFilterInfo.errorInEstimate));
	}
}

void calculateKalmanGain(KalmanFilterInformation *kalmanFilterInfo, short measurement) {
	kalmanFilterInfo->kalmanGain = kalmanFilterInfo->errorInEstimate / (kalmanFilterInfo->errorInEstimate + kalmanFilterInfo->errorInMeasurement);
}

void calculateKalmanEstimate(KalmanFilterInformation *kalmanFilterInfo, short measurement) {
	kalmanFilterInfo->previousEstimate = kalmanFilterInfo->currentEstimate;
	kalmanFilterInfo->currentEstimate = kalmanFilterInfo->previousEstimate + kalmanFilterInfo->kalmanGain * (measurement - kalmanFilterInfo->previousEstimate);
}

void calculateKalmanErrorInEstimate(KalmanFilterInformation *kalmanFilterInfo) {
	kalmanFilterInfo->previousErrorInEstimate = kalmanFilterInfo->errorInEstimate;
	kalmanFilterInfo->errorInEstimate = (1 - kalmanFilterInfo->kalmanGain)*kalmanFilterInfo->previousErrorInEstimate;
}
