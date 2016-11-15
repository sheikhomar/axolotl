// 
// 
// 

#include "Defines.h"

#include "SerialController.h"
#include "Utilities.h"

//External libs
#include <SoftwareSerial.h>

//Objects
SoftwareSerial RS485Serial(RS485_RX, RS485_TX);


/***************************
serialSetup

Initialize serial communication
***************************/
void serialSetup(long mainBaud, long debugBaud) {
	RS485Serial.begin(mainBaud);
	Serial.begin(debugBaud);

	pinMode(SERIAL_TRANSMIT_PIN, OUTPUT);
	digitalWrite(SERIAL_TRANSMIT_PIN, LOW);
}

/***************************
serialCheck

Returns the first client waiting to be read on the Rs485 buffer.
If no data exists in buffer none is returned.
***************************/
client serialCheck() {
	int read;

	if (RS485Serial.available() < 1) {
		return none;
	}

	read = RS485Serial.peek();

	switch (read)
	{
	case unknown:
	case RaspberryPi:
	case NXT:
	case Debug_Client:
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

	#if DEBUG
		serialWrite(receiver);
		serialWrite(sizeOfData);
		serialWrite(reciverFunction);
		for (i = 0; i < sizeOfData; i++) {
			serialWrite(data[i]);
		}
	#endif
}

void serialSendData(client receiver, byte reciverFunction) {
	serialSendData(receiver, NULL, 0, reciverFunction);
}

/***************************
serialReadData

Reads a RS485 message from the network and saves it in the given data array.
***************************/
client serialReadData(byte data[], int data_length, int *command) {
	int id, length;
	int i;
	bool incorrectSender = false;

    // We expect the package size to always be 4
    if (RS485Serial.available() <= 3) {
        return none;
    }

	//Noise handle
	do
	{
        if (RS485Serial.available() < 1) {
            return unknown;
        }

		id = RS485Serial.read();

		#if (DEBUG)
		  serialWrite(id);
		#endif

		incorrectSender = !(id == Debug_Client || id == NXT || id == RaspberryPi || id == Arduino);
	} while (incorrectSender);

	//Read length and command
	serialBuffTimeout(2); 
	length = RS485Serial.read();
	*command = RS485Serial.read();


	#if DEBUG
		serialWrite(length); 
		serialWrite(*command);
	#endif
    
	//Kick if length is incorrect
	if (length < 0 || length > RS485_DATA_LENGTH_MAX) {
		return unknown;
	}

    if (length != data_length) {
        die("length does not match!");
    }
	serialBuffTimeout(length);

	//Read non-Arduino messages
	if (id != Arduino) {
		for (i = 0; i < length; i++)
		{
			#if DEBUG
				serialWrite(RS485Serial.read());
			#else
				RS485Serial.read()
			#endif
		}
		return (client)id;
	}

	//Handle Arduino message
	for (i = 0; i < length; i++)
	{
		data[i] = RS485Serial.read();
		#if DEBUG
			serialWrite(data[i]);
		#endif
	}

    return (client)id;
}

client serialReadData(byte data[], int data_length) {
    int command;
    return serialReadData(data, data_length, &command);
}

/***************************
serialBuffTimeout

Continuously checks the RS485 buffer to see if # of required elements are present.
***************************/
bool serialBuffTimeout(int numberOfRequiredElements) {
	return true; //todo, make this work as described above.
	
	unsigned long time = micros() + RS485_TIMEOUT_US;
	bool reachedTarget, reachedTimeout;

	do
	{
		reachedTimeout = time < micros();
		reachedTarget = numberOfRequiredElements <= Serial.available();
        
	} while (!reachedTarget && !reachedTimeout);

    if (reachedTimeout) {
        serialDebugLN("Buffer timeout!");
    }
	return reachedTarget;
}

/***************************
serialDebug

Broadcasts a RS485 message with a special DEBUG tag.
***************************/
void serialDebug(String message) {
	#if DEBUG
		Serial.print(message);
	#endif
}

void serialDebugLN(String message) {
	serialDebug(message);
	serialDebug("\r\n");
}

/***************************
serialWrite

Writes a byte to the DEBUG serial
***************************/
void serialWrite(int data) {
#if RS485_SERIAL_PRINT_BINARY
	Serial.write(data);
#else
	if (data < 33 || data > 126) {
		Serial.write('|');
		Serial.print(data);
		Serial.write('|');
	}
	else {
		Serial.write(data);
	}
#endif
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
		RS485Serial.write('a' + i);
		serialWrite('a' + i);
	}

	RS485Serial.write('_');
	RS485Serial.write('_');
	serialWrite('_');
	serialWrite('_');

	for (i = 0; i < 26; i++)
	{
		numb = rand();
		RS485Serial.write(numb);
		serialWrite(numb);
	}

	digitalWrite(SERIAL_TRANSMIT_PIN, LOW);
	serialDebugLN("");
	serialDebugLN("Done");
}