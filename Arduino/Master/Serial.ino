void serialSendData(client reciver, byte data[], byte sizeOfData, byte reciverFunction) {
	int i = 0;

	//if() check that data is nonempty, if data fits buffer...

	//handshake


	digitalWrite(SerialTransmitPin, HIGH);

	Serial.write(reciver);
	Serial.write(sizeOfData + 1);
	Serial.write(reciverFunction);

	for (i = 0; i < sizeOfData; i++) {
		Serial.write(data[i]);
		//delay(10);
	}
	digitalWrite(SerialTransmitPin, LOW);
}


void serialReadData(bool readAll) {
	byte id, length, command, data[serialMaxDataSize];
	int i;

	//read id and length
	id = Serial.read();
	length = Serial.read();

	if (id == -1)
		return;

	if (id != Arduino) {
		for (i = 0; i < length; i++)
		{
			Serial.read();
		}
		return;
	}

	//save Arduino message
	command = Serial.read();
	for (i = 0; i < length - 1; i++)
	{
		data[i] = Serial.read();
	}

	switch (command)
	{
	case 0:
		//do stuff
		break;
	default:
		break;
	}
}



void serialCountTest()
{
	int i, sizeOfData = 26;
	byte myArr[sizeOfData];

	for (i = 0; i<sizeOfData; i++) {
		myArr[i] = 'a' + i;
	}

	serialSendData(NXT, myArr, sizeOfData, 0);
}
