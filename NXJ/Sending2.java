import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;

public class Sending2 {
	public static void main(String[] args) throws InterruptedException {
		int succesful = 0, i = 0;
		byte[] sendBuff = new byte[1];
		byte[] recBuff = new byte[1];
		sendBuff[0] = (byte)11;
		recBuff[0] = (byte)0;
		//sendBuff[1] = 'a';
		//System.out.println("Sending " + sendBuff[0]);
		RS485.hsEnable(57600, 0);
		while(true){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length);
			if(succesful == 1){
				System.out.println("Sending " + sendBuff[0]);
				sendBuff[0] += 1;
			}
			Delay.msDelay(500);
		}
	}
}