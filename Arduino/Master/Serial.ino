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
	bool incorrectSender = false;


	//Noise handle
	do
	{
		if (RS485Serial.available() < 1) {
			return none;
		}
		id = RS485Serial.read();
		{Serial.write(id);} //Additional debug messages

		incorrectSender = !(id == DEBUG || id == NXT || id == RaspberryPi || id == Arduino);
	} while (incorrectSender);
  
	//Read length and command
	serialBuffTimeout(2);
	length = RS485Serial.read();
	command = RS485Serial.read();
	{Serial.write(length);Serial.write(command);}//Additional debug messages

	//Kick if length is incorrect
	if (length < 0 || length > RS485_DATA_LENGTH_MAX) {
		return unknown;
	}
	serialBuffTimeout(length);

	//Read non-Arduino messages
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
	case   0: break;
	case   5: break;  //NXT colour
	case  10: break; //Reserved for DEBUG
	case 'p': break; //PI motor to push
	default:
		break;
	}

	return Arduino;
}
/***************************
serialBuffTimeout

Continuously checks the RS485 buffer to see if # of required elements are present.
***************************/
bool serialBuffTimeout(byte numberOfRequiredElements) {
	long time = micros();
	bool reachedTarget, reachedTimeout;

	do
	{
		reachedTimeout = time + RS485_TIMEOUT_US < micros();
		reachedTarget = numberOfRequiredElements <= Serial.available();
	} while (!reachedTarget && !reachedTimeout);

	return reachedTarget;
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


////////////////////////////// TEST CODE //////////////////////////////


/***************************
serialSendTest

Broadcasts the alphabet out on the RS485 network
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
	byte data[] = { 'a' };
	client sender = unknown;

	while (data[0] <= 'y')
	{
		serialSendData(NXT, data, 1, 10);

		do {
			sender = serialReadData(data, 1);
		} while (sender != Arduino);
		String masterString = "|Data=";
		masterString.concat(data[0]);
		masterString.concat("|");

		data[0] += 1;
	}
}

/***************************
serialArduinoPICommTest

Combines the sending and receiving of data together with the PI.
Sends five packages to the PI and blinks the builtin LED in acc with which motor to push.
***************************/
void serialArduinoPICommTest() {
	byte packages[5][4] = { 
		{ 1,1,1,COLOUR_RED},
		{ 2,2,2,COLOUR_BLUE},
		{ 4,4,4,COLOUR_YELLOW},
		{ 8,8,8,COLOUR_YELLOW},
		{ 1,2,3,COLOUR_GREEN} 
		};
	byte i;

	for (i = 0; i < 5; i++)
	{
		serialArduinoPICommTestHelperFunction(packages[i]);
	}
}

void serialArduinoPICommTestHelperFunction(byte data[]) {
	client sender = unknown;
	byte received[] = { 0 };

	serialSendData(RaspberryPi, data, 4, COMM_PI_ADVICEPACKAGE);

	do {
		sender = serialReadData(received, 1);
	} while (sender != RaspberryPi);


	while (received[0] > 0) {
		LED(LED_BUILTIN, true);
		delay(500);
		received[0] -= 1;
	}
}

/***************************
serialNoiseMaker

Writes the alphabet and then 26 sudo-random values out on the RS485 network
***************************/
void serialNoiseMaker() {
	int i, numb;
	srand(42);

	serialDebugLN("Writing crap now");

	digitalWrite(SERIAL_TRANSMIT_PIN, HIGH);
	for (i = 0; i < 26; i++)
	{
		RS485Serial.write('a'+i);
    Serial.write('a'+i);
	}

	RS485Serial.write('_');
	RS485Serial.write('_');
    Serial.write('_');
    Serial.write('_');

	for (i = 0; i < 26; i++)
	{
		numb = rand();
		RS485Serial.write(numb);
    Serial.write(numb);
	}

	digitalWrite(SERIAL_TRANSMIT_PIN, LOW);
	serialDebugLN("");
	serialDebugLN("Done");
}