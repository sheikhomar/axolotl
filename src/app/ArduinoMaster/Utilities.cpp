// 
// 
// 

#include "Defines.h"

#include "Utilities.h"


void die(String abortMessage) {
	serialDebug(abortMessage);
	while (1);
}

void runConveyorBeltAtSpeed(byte speed) {
	byte data[1] = { speed };
	serialSendData(NXT, data, 1, COMM_NXT_ADJUST_BELT_SPEED);
	serialSendData(NXT, data, 0, COMM_NXT_START_BELT);
}
