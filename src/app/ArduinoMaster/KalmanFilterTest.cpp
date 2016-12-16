// 
// 
// 
#include "KalmanFilter.h"
#include "SerialController.h"

#include "KalmanFilterTest.h"


void KalmanFilterTestRun() {
	//initKalmanFilter
	initKalmanFilterTest();

	//KalmanGain




	//KalmanEstiamte




	//KalamnErrorInEstimate




	//ResetKalmanFilter




	//update prediction

}


//Testing if the kalman filter is initialized correct
void initKalmanFilterTest() {
	KalmanFilterInformation kfi;
	kfi.currentEstimate = 1;
	kfi.errorInEstimate = 2;
	kfi.kalmanGain = 3;
	kfi.processNoise = 4;
	kfi.sensorNoise = 5;

	double errorInEsitmate = 10, sensorNoise = 11, currentEstimate = 12, processNoise = 13;

	initKalmanFilter(&kfi, errorInEsitmate, sensorNoise, currentEstimate, processNoise);

	if (
		kfi.currentEstimate != currentEstimate ||
		kfi.errorInEstimate != errorInEsitmate ||
		kfi.kalmanGain != 0 ||
		kfi.processNoise != processNoise ||
		kfi.sensorNoise != sensorNoise) {
			serialDebugLN("KalmanFilter was not correctly initialized.");
	}
}