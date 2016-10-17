#define TOWAIT 2000
#define MSG "Hello Arduino  "

// -------------------------------------------------------------
void printMsg(byte inbuffer[], int cnt) {
  TextOut(0, LCD_LINE3, "       Baud");
  NumOut(0, LCD_LINE3, 57600);
  TextOut(0, LCD_LINE5, "l=    cnt=     ");
  NumOut(12, LCD_LINE5, ArrayLen(inbuffer));
  NumOut(60, LCD_LINE5, cnt);
  //                        "Hi NXT,got  "
  TextOut(0, LCD_LINE6, "rcv:             ");
  TextOut(24, LCD_LINE6, ByteArrayToStr(inbuffer));
}

// -------------------------------------------------------------
task main() {
  int i = 0;
  int cnt = 0;
  byte outbuffer[];
  byte inbuffer[];
  char result;

  SetSensorType(IN_4, SENSOR_TYPE_HIGHSPEED);
  Wait(100);

  // we use odd parity on the Arduino side
  // see: http://forums.nxtasy.org/index.php?showtopic=3871
  RS485Uart(HS_BAUD_##57600, HS_MODE_8_DATA|HS_MODE_O_PARITY|HS_MODE_10_STOP);
  Wait(100);
  StrToByteArray(MSG, outbuffer);
  TextOut(0, LCD_LINE1, "RS485 <> Arduino");

  while (true) {
    outbuffer[14] = '0' + i;
    i++; if (i==10) i = 0;

    result = RS485Write(outbuffer);
    while (!RS485DataAvailable()) {}
    result = RS485Read(inbuffer);

    printMsg(inbuffer, cnt);
    cnt++; if (cnt==1000) cnt = 0;
    Wait(TOWAIT);
  }
}
