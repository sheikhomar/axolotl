void SerialCountTest()
{
	int i;

	digitalWrite(SerialTransmitPin, RS485Transmit);  // Enable RS485 Transmit   

	for (i = 'a'; i< 'a' + 26; i++) {
		Serial.write((byte)i);          // Send byte to Remote Rs485 devices
		delay(10);
	}

	digitalWrite(SerialTransmitPin, RS485Receive);  // Disable RS485 Transmit         

	while (true);
}