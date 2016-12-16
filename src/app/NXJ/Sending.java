import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;

public class Sending {
	public static void main(String[] args) throws InterruptedException {
		int succesful = 0, i = 0;
		byte[] sendBuff = new byte[4];
		sendBuff[0] = (byte)97;
		sendBuff[1] = (byte)1;
		sendBuff[2] = (byte)10;
		sendBuff[3] = (byte)0;
		
		RS485.hsEnable(57600, 0);
		while(true){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length);
			if(succesful != 0){
				System.out.println("S " + sendBuff[0] + ", " + sendBuff[1] + ", " + sendBuff[2] + ", " + sendBuff[3]);
				sendBuff[3] += 1;
			}
			Delay.msDelay(500);
		}
	}
}