import lejos.nxt.*;

public class MotorArduOld {
    static int I2CSlaveAddress = 8;
    //static byte[] buffReadResponse = new byte[8];

    public static void main(String[] args) throws InterruptedException {
        System.out.println("Arduino Connection Test");
        I2CSensor arduino = new I2CSensor(SensorPort.S1, I2CSlaveAddress);
        byte[] sendBuff = new byte[2];
		byte[] recBuff = new byte[1];
		int succesful = 0, speed = 0;
		
		Motor.C.resetTachoCount();
		Motor.C.setSpeed(50);
		sendBuff[1] = (byte)speed;
		recBuff[0] = (byte)0;

        while (true) {
			sendBuff[0] = (byte)(Motor.C.getTachoCount() % 128);
			
			if(speed == 0){
				sendBuff[1] = 0;
			}
			else if(speed > 0){
				sendBuff[1] = 1;
			}
			else{
				sendBuff[1] = -1;
			}
			//System.out.println("dir: " + sendBuff[1] + " t: " + sendBuff[0]);
            arduino.sendData(0, sendBuff, sendBuff.length);
            //Thread.sleep(1000);

            succesful = arduino.getData('S', recBuff, recBuff.length);
			if(succesful == 0){
				
				switch(recBuff[0]){
					case -1: Motor.C.backward(); speed = -1; break;
					case 1: Motor.C.forward(); speed = 1; break;
					default: break;
				}
			}
			else{
				System.out.println("Succes: " + Integer.toString(succesful));
			}

            System.out.println("rec: " + recBuff[0] + " sp: " + Integer.toString(speed) + " t: " + Byte.toString(sendBuff[0]));
			//Thread.sleep(500);
        }
    }
}