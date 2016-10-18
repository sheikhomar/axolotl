#define SSerialTxControl 12   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

int byteReceived;
int byteSend;

void setup()
{
  Serial.begin(57600);
  pinMode(SSerialTxControl, OUTPUT);    
  
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver   
}


void loop()
{
  int i;

  digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit   
  
  for(i = 'a'; i< 'a' + 26; i++){
    Serial.write((byte)i);          // Send byte to Remote Rs485 devices
    delay(10);
  }

  digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit         

    while (true);
}
