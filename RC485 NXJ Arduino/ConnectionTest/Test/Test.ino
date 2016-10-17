// Pins and wires
//         Arduino                Wire       MAX485            
const byte RS485_OUT_PIN = 10; // yellow --> DI
const byte RS485_IN_PIN  = 11; // green  --> RO
const byte RS485_CTL_PIN = 12; // grey   --> RE+DE
//         VCC                           --> VCC
//         ------------------------------------------
//         NXT Sensor Port 4      Wire       MAX485
//         DIGIAI1                blue   --> B
//         DIGIAI0                yellow --> A
//         GND                    black  --> GND
 
const byte RECV_RS485_BIT =  B00001000;
const byte SEND_RS485_BIT =  B00000100;
#define readRS485inPin (PINB & RECV_RS485_BIT)
 
const unsigned int BUFLEN = 128;
const unsigned int outnbuf = 12;
byte outBuf[BUFLEN];
byte inBuf [BUFLEN];
unsigned int innbuf;
const char msg[] = "Hi NXT,got  ";
 
const byte SER_TIMEOUT = 7;
unsigned int RS485_LEN;
unsigned int bitDelay;
unsigned int halfBitDelay;
 
const byte CYCLES = 45;
// ------------------------------------------------------------------
void serSetup(unsigned long baud) {
  pinMode(RS485_OUT_PIN, OUTPUT);
  digitalWrite(RS485_OUT_PIN, LOW);
  pinMode(RS485_IN_PIN, INPUT);
  digitalWrite(RS485_IN_PIN, LOW);
  pinMode(RS485_CTL_PIN, OUTPUT);
  digitalWrite(RS485_CTL_PIN, LOW);
 
  RS485_LEN = 1000000 / baud;
  bitDelay = RS485_LEN - clockCyclesToMicroseconds(CYCLES);
  halfBitDelay = RS485_LEN/2 - clockCyclesToMicroseconds(CYCLES);
  Serial.print("\nRS-485 configured, ");
  Serial.print(baud, DEC);
  Serial.print(" Baud, Bit length ");
  Serial.print(RS485_LEN, DEC);
  Serial.println(" uSec");
}
 
// Startbit of 1st byte of incoming message encountered
// ------------------------------------------------------------------
byte serAvail() {
  return readRS485inPin;
}
 
// Read a byte
// ------------------------------------------------------------------
unsigned int serRead() {
  byte i;
  int val = 0;
  byte parity = 0, stopbit = 0;
 
  long start = millis();
  while (true) {
    if (millis()-start>SER_TIMEOUT) return -1;
    if (readRS485inPin) break;     // startbit hit
  }
 
  delayMicroseconds(halfBitDelay); // jump to middle of startbit
  for (i=0; i<8; i++) {
    delayMicroseconds(bitDelay);
    val |= !readRS485inPin << i;   // data bits
  }
  delayMicroseconds(bitDelay);
  parity = !readRS485inPin;        //
  delayMicroseconds(bitDelay);
  stopbit = !readRS485inPin;       //
  return val;                                               
}                                                             
 
// Write one bit
// -----------------------------------------------------------------
void serBit(byte mark) {
  if (mark) PORTB &= ~ SEND_RS485_BIT;
  else      PORTB |=   SEND_RS485_BIT;
//  delayMicroseconds(RS485_LEN);
  delayMicroseconds(bitDelay);
}
 
// Write a byte
// ------------------------------------------------------------------
void serWrite(byte data) {
  byte mask = 1;
  byte bitcount = 0;
 
  serBit(LOW);               // startbit
  for (byte i=0; i<8; i++) { // data from LSB to MSB
    if (data & mask) {
      serBit(HIGH);
      bitcount++;
    } else {
      serBit(LOW);
    }
    mask <<= 1;
  }
  serBit((bitcount%2)==0);   // odd parity bit
  serBit(HIGH);              // stop bit
}
 
// ------------------------------------------------------------------
void printMsg(const char *s, byte buf[], int nbuf) {
  Serial.print(s);
  Serial.print(": \"");
  for (int i=0; i<nbuf; i++)  
    Serial.write(buf[i]);
  Serial.println("\"");
}
 
// ------------------------------------------------------------------
void serSendMsg(byte buf[], int nbuf) {
  int i;
 
  digitalWrite(RS485_CTL_PIN, HIGH);
  for (i=0; i<nbuf; i++)
    serWrite(buf[i]);
  digitalWrite(RS485_CTL_PIN, LOW);
}
 
// ------------------------------------------------------------------
int serRecvMsg(byte buf[]) {
  int c;
  int nbuf = 0;
 
  while ((c = serRead())!=-1)
    buf[nbuf++] = c;
  return nbuf;
}
 
// ------------------------------------------------------------------
void measureComms(byte stage) {
  static unsigned long old = 0; 
  unsigned long now; 
 
  if (!stage) {
    old = millis();
  } else {
    now = millis();
    Serial.print("comms took ");
    Serial.print(now-old, DEC);
    Serial.println(" mSec");
  } 
}
 
// ------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  for (int i=0; i<outnbuf; i++) 
    outBuf[i] = msg[i];
  outBuf[12] = 0;
  serSetup(921600);
}
 
// ------------------------------------------------------------------
void loop() {
//  measureComms(0);
  if (serAvail()) {
    innbuf = serRecvMsg(inBuf);
    outBuf[11] = inBuf[innbuf-1];
    serSendMsg(outBuf, outnbuf);
 
//    measureComms(1);
    printMsg("Recv", inBuf, innbuf);
    printMsg("Sent", outBuf, outnbuf);
  }
}
