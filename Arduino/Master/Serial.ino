/***************************
serialCheck

Returns the first client waiting to be read on the Rs485 buffer.  
If no data exists in buffer none is returned.
***************************/
client serialCheck() {
	byte read;

	read = RS485Serial.peek();

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

/***************************
serialSendData

Broadcasts a RS485 message to the specified receiver (of type client), starting the given function supplied with required data.
***************************/
void serialSendData(client receiver, byte data[], byte sizeOfData, byte reciverFunction) {
	int i = 0;

	digitalWrite(SERIAL_TRANSMIT_PIN, HIGH);

	RS485Serial.write(receiver);
	RS485Serial.write(sizeOfData);
	RS485Serial.write(reciverFunction);

	for (i = 0; i < sizeOfData; i++) {
		RS485Serial.write(data[i]);
	}
	digitalWrite(SERIAL_TRANSMIT_PIN, LOW);

	//Additional debug messages
	{
		Serial.write(receiver);
		Serial.write(sizeOfData);
		Serial.write(reciverFunction);
		Serial.write("_");
		for (i = 0; i < sizeOfData; i++) {
			Serial.write(data[i]);
		}
	}
}

void serialSendData(client receiver, byte reciverFunction) {
	serialSendData(receiver, NULL, 0, reciverFunction);
}

/***************************
serialReadData

Reads a RS485 message from the network and saves it in the given data array.
***************************/
client serialReadData(byte data[], int data_length) {
	byte length, command;
	int id, i;

	//Noise handle
	id = RS485Serial.read();

	if (id == -1){
		return unknown;
	}

	Serial.write(id); //Additional debug messages

	if (id != DEBUG && id != NXT && id != RaspberryPi && id != Arduino) { //make it so it's not recursive
		return serialReadData(data, data_length);
	}

  delay(1000); //debug should be changed so we can handle reads of -1 and being stuck


	//Read length and command
	length = RS485Serial.read();
	command = RS485Serial.read();

	//Additional debug messages
	{
		Serial.write(length);
		Serial.write(command);
	}

	if (id != Arduino) {
		for (i = 0; i < length; i++)
		{
			Serial.write(RS485Serial.read()); //debug, should only read not print out.
		}
		return (client)id;
	}

	//Handle Arduino message
	for (i = 0; i < length; i++)
	{
		data[i] = RS485Serial.read();
		Serial.write(data[i]);
	}

	switch (command)
	{
	case 0:
		//do stuff
		break;
  case 10: //Arduino NXT talk
    return Arduino;
	default:
		break;
	}

	return Arduino;
}

/***************************
serialDebug

Broadcasts a RS485 message with a special DEBUG tag.
***************************/
void serialDebug(String message) {
	Serial.print(message);
	/*
	int msgLen = message.length() + 1;
	byte data[msgLen];
	message.getBytes(data, msgLen);
	serialSendData(DEBUG, data, msgLen, '_');*/
}

void serialDebugLN(String message) {
	message.concat("\r\n");
	serialDebug(message);
}

/***************************
serialSendTest


***************************/
void serialSendTest()
{
	int i, sizeOfData = 26;
	byte myArr[sizeOfData];

	for (i = 0; i<sizeOfData; i++) {
		myArr[i] = 'a' + i;
	}

	serialSendData(NXT, myArr, sizeOfData, 0);
}

/***************************
serialReceiveTest


***************************/
void serialReceiveTest() {
	byte byteReceived[] = {0};

	if (RS485Serial.available())
	{
		LED(LED1_PIN, true);
		serialReadData(byteReceived, 1);
		serialDebug(String(byteReceived[0]));
		delay(10);
		LED(LED1_PIN, false);
	}
}

/***************************
serialArduinoNXTLoopTest

Combines the sending and receiving of data together with the NXT. 
A byte is incremented from 'a' to 'z', where both the Arduino and NXT increments the value by one.
***************************/
void serialArduinoNXTLoopTest() {
	byte data = 'a';
	client sender = unknown;

	while (data <= 'y')
	{
		serialSendData(NXT, &data, 1, 10);

		do {
			sender = serialReadData(&data, 1);
			delay(10);
		} while (sender != Arduino);
		String masterString = "|Data=";
		masterString.concat(data);
		masterString.concat("|");

		data += 1;
	}
}