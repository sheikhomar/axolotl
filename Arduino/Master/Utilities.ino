
void die(String abortMessage) {
	serialDebug(abortMessage);
	while (1);
}

void runConveyorBeltAtSpeed(byte speed) {
	byte data[1] = { speed };
	serialSendData(NXT, data, 1, 3);
}