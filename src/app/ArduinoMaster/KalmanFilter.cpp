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
	short falseCount = 0;
	short count = 0;
	bool constantReadingMode = false;
	bool running = false;
	bool first = true;

	kalmanFilterInfo.kalmanGain = 0;
	kalmanFilterInfo.errorInEstimate = 300;
	kalmanFilterInfo.previousErrorInEstimate = 0;
	kalmanFilterInfo.errorInMeasurement = 100;
	kalmanFilterInfo.currentEstimate = 3200;
	kalmanFilterInfo.previousEstimate = 0;

	while (true) {
		delay(5);
		measurement = (double) GetUltDistance(ULT1_TRIG_PIN, ULT1_ECHO_PIN, false);

		if (measurement <= 3800 || running || constantReadingMode) {
			count += 1;

			running = true;
			first = true;
			if (running && !(measurement <= 3800)) {
				falseCount += 1;
				//serialDebug("Kalman Estimate: " + String(kalmanFilterInfo.currentEstimate));
				//serialDebugLN(" Error in Estimate: " + String(kalmanFilterInfo.errorInEstimate));
				//serialDebugLN(" falseCount: " + String(falseCount));
				if (!constantReadingMode) {
					if (falseCount == 3)
						running = false;
				}
			}
			else
				falseCount = 0;

			calculateKalmanGain(&kalmanFilterInfo, measurement);
			calculateKalmanEstimate(&kalmanFilterInfo, measurement);
			calculateKalmanErrorInEstimate(&kalmanFilterInfo);
			
			if (constantReadingMode) {
				serialDebug("Measured distance: " + String(measurement));
				//serialDebug(" Kalman Gain: " + String(kalmanFilterInfo.kalmanGain));
				serialDebug(" Kalman Estimate: " + String(kalmanFilterInfo.currentEstimate));
				serialDebugLN(" Error in Estimate: " + String(kalmanFilterInfo.errorInEstimate));
			}
		}
		else {
			if (first && count > 20) {
				serialDebugLN("========= DONE ==========");
				serialDebug("Kalman Estimate: " + String(kalmanFilterInfo.currentEstimate));
				serialDebug(" Error in Estimate: " + String(kalmanFilterInfo.errorInEstimate));
				serialDebug(" falseCount: " + String(falseCount));
				short height = 4769.29 - kalmanFilterInfo.currentEstimate;
				serialDebug(" Height: " + String(height));
				serialDebugLN(" count: " + String(count) + "\n\n");
				
			}
			kalmanFilterInfo.kalmanGain = 0;
			kalmanFilterInfo.errorInEstimate = 300;
			kalmanFilterInfo.previousErrorInEstimate = 0;
			kalmanFilterInfo.errorInMeasurement = 100;
			kalmanFilterInfo.currentEstimate = 3200;
			kalmanFilterInfo.previousEstimate = 0;

			count = 0;
			falseCount = 0;
			first = false;
		}
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
