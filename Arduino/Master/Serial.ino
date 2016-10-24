client serialCheck() {
	byte read;

	read = Serial.peek();

	switch (read)
	{
		case unknown:
		case RaspberryPi:
		case NXT:
		case DEBUG:
		case Arduino:
		return (client)read;

		default:
			return unknown;
	}

 return unknown;
}


void serialSendData(client reciver, byte data[], byte sizeOfData, byte reciverFunction) {
	int i = 0;

	//if() check that data is nonempty, if data fits buffer...

	//handshake


	digitalWrite(SerialTransmitPin, HIGH);

	Serial.write(reciver);
	Serial.write(sizeOfData);
	Serial.write(reciverFunction);

	for (i = 0; i < sizeOfData; i++) {
		Serial.write(data[i]);
		//delay(10);
	}
	digitalWrite(SerialTransmitPin, LOW);
}

void serialDebug(String message) {
  int msgLen = message.length()+1;
  byte data[msgLen];
  message.getBytes(data, msgLen);
  serialSendData(DEBUG, data, msgLen, '-');
}

void serialDebugLN(String message) {
	serialDebug(message + '\n');
}

void serialReadData(byte data[], int data_length) {
	byte id, length, command;
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
  case 10: //Arduino NXT talk
    return;
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

void serialArduinoNXTTalk() {
	byte data = 'a';

	//serialDebugLN("Starting Arduino NXT talk..");
	delay(2000);

	while (data <= 'y')
	{
		serialSendData(NXT, &data, 1, 10);
		while (serialCheck() != NXT) {
			serialReadData(&data, 1);
		}
    data += 1;

		serialReadData(&data, 1);
	}

	serialDebugLN("Finished..");
}
