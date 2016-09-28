#include <Wire.h>
 
#define SLAVE_ADDRESS 1
void setup() 
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}
 
byte request;
byte reply[2] = {0,0};
 
void loop(){
}
 
void receiveData(int byteCount){
  request = Wire.read();
  if(request != 'A' && request != 'S')
  {
    int i = 0;
    while (0 < Wire.available()) 
    {
      reply[i++] = Wire.read(); 
    }
  }
}
 
void sendData(){
  if (request == 'A'){
    reply[0] += reply[1];
    Wire.write(reply,sizeof reply);
  }
  else if(request == 'S'){
    reply[0] -= reply[1];
    Wire.write(reply,sizeof reply);
  }
  else{
   byte error[] = {'E'};  
    Wire.write(error , sizeof error);
  }
}
