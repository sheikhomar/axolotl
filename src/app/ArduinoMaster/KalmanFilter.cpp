// 
// 
// 
#include "Defines.h"
#include "Utilities.h"
#include "SerialController.h"
#include "UltrasoundSensor.h"

#include "KalmanFilter.h"

void updateKalmanFilter(KalmanFilterInformation *kfi, double measurement){
    updatePrediction(kfi);						//Calculates P_predict
    calculateKalmanGain(kfi);					//Calculates K
    calculateKalmanEstimate(kfi, measurement);	//Calculates X_corr
    calculateKalmanErrorInEstimate(kfi);		//Calculates P_coor
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
