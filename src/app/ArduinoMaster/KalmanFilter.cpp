// 
// 
// 
#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "KalmanFilter.h"

void updateKalmanFilter(KalmanFilterInformation *kfi, double measurement){
    updatePrediction(kfi);
    calculateKalmanGain(kfi);
    calculateKalmanEstimate(kfi, measurement);
    calculateKalmanErrorInEstimate(kfi);
}

void kalmanTestFunction() {
	KalmanFilterInformation kalmanFilterInfo;
	double measurement = 0;
	short falseCount = 0;
	short count = 0;
	bool constantReadingMode = true;
	bool running = false;
	bool first = true;
	
	resetKalmanFilter(&kalmanFilterInfo);

	while (true) {
		delay(5);
		measurement = (double) GetUltDistance(ULT_TOP_TRIG_PIN, ULT_TOP_ECHO_PIN, false);

		if (measurement <= 3800 || running || constantReadingMode) {
			count += 1;

			running = true;
			first = true;
			if (running && !(measurement <= 3800)) {
				falseCount += 1;

				if (!constantReadingMode) {
					if (falseCount == 3)
						running = false;
				}
			}
			else
				falseCount = 0;

			updatePrediction(&kalmanFilterInfo);
			calculateKalmanGain(&kalmanFilterInfo);
			calculateKalmanEstimate(&kalmanFilterInfo, measurement);
			calculateKalmanErrorInEstimate(&kalmanFilterInfo);
			
			if (constantReadingMode) {
				serialDebug("Measured distance: " + String(measurement));
				//serialDebug(" Kalman Gain: " + String(kalmanFilterInfo.kalmanGain));
				serialDebug(" Kalman Estimate: " + String(kalmanFilterInfo.currentEstimate));
				//serialDebug(" Error in Estimate: " + String(kalmanFilterInfo.errorInEstimate));
				serialDebug("\n");
			}
		}
		else {
			if (first && count > 20) {
				serialDebugLN("========= DONE ==========");
				serialDebug("Kalman Estimate: " + String(kalmanFilterInfo.currentEstimate));
				serialDebug(" Error in Estimate: " + String(kalmanFilterInfo.errorInEstimate));
				serialDebug(" falseCount: " + String(falseCount));
				//short height = 4769.29 - kalmanFilterInfo.currentEstimate;
				//serialDebug(" Height: " + String(height));
				//serialDebugLN(" count: " + String(count) + "\n\n");
				
			}
			
			resetKalmanFilter(&kalmanFilterInfo);

			count = 0;
			falseCount = 0;
			first = false;
		}
	}
}

void resetKalmanFilter(KalmanFilterInformation *kfi) {
	kfi->kalmanGain = 0;
	kfi->errorInEstimate = 5000;
	kfi->sensorNoise = 200;
	kfi->currentEstimate = 4000;
	kfi->processNoise = 10;
}


void initKalmanFilter(KalmanFilterInformation *kfi, double errorInEstimate, double sensorNoise, double currentEstimate, double processNoise) {
    kfi->kalmanGain = 0;
    kfi->errorInEstimate = errorInEstimate;
    kfi->sensorNoise = sensorNoise;
    kfi->currentEstimate = currentEstimate;
    kfi->processNoise = processNoise;
}

void calculateKalmanGain(KalmanFilterInformation *kalmanFilterInfo) {
	kalmanFilterInfo->kalmanGain = kalmanFilterInfo->errorInEstimate / (kalmanFilterInfo->errorInEstimate + kalmanFilterInfo->sensorNoise);
}

void calculateKalmanEstimate(KalmanFilterInformation *kalmanFilterInfo, short measurement) {
	kalmanFilterInfo->currentEstimate = kalmanFilterInfo->currentEstimate + kalmanFilterInfo->kalmanGain * (measurement - kalmanFilterInfo->currentEstimate);
}

void calculateKalmanErrorInEstimate(KalmanFilterInformation *kalmanFilterInfo) {
	kalmanFilterInfo->errorInEstimate = (1 - kalmanFilterInfo->kalmanGain)*kalmanFilterInfo->errorInEstimate;
}

void updatePrediction(KalmanFilterInformation *kalmanFilterInfo) {
	kalmanFilterInfo->errorInEstimate += kalmanFilterInfo->processNoise;
}
