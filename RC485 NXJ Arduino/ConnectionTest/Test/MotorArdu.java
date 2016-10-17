import lejos.nxt.*;
import lejos.util.Delay;
import lejos.nxt.comm.RS485;

public class MotorArdu {
	private static NXTMotor ma = new NXTMotor(MotorPort.A);
	private static NXTMotor mb = new NXTMotor(MotorPort.B);
	private static NXTMotor mc = new NXTMotor(MotorPort.C);
    static String RS485Address = "8";

    public static void main(String[] args) throws InterruptedException {
        System.out.println("Arduino Connection Test");
		//LCD.drawString("A-mei-zing!",3,3);
        byte[] sendBuff = new byte[1];
		byte[] recBuff = new byte[1];
		int succesful = 0, count = 0, countMax = 6;;
		String recString = new String("");
		sendBuff[0] = (byte)0;
		recBuff[0] = (byte)0;
		
		
		RS485.hsEnable(57600, 0);
		
		while(true){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			Delay.msDelay(100);
			if(succesful == 1){
				//if(recBuff[0] != 0){
					//count++;
					//if(recBuff[0] == 42){
						System.out.println("Rec: " + Byte.toString(recBuff[0]));
						
						recString = determineRecString(recBuff);
						control(recString);
						//System.out.println("I'm here");
						//sendBuff[0] = (byte)('a');
						succesful = 0;
						//while(succesful == 0){
							//succesful = RS485.hsWrite(recBuff, 0, recBuff.length);
							//if(succesful == 1){
							//	System.out.println("Send: " + Byte.toString(recBuff[0]));
							//}
							//else{
							//	System.out.println("No send: " + Integer.toString(succesful));
							//}
						//}
					//}
					//else{
						
					//	System.out.println("Rec: " + Byte.toString(recBuff[0]));
					//}
				//}
			}
			else{
				//System.out.println("No rec, bytes: " + Integer.toString(succesful));
			}
		}
		
		//String[] testInput = {"A"};
		//control(testInput);
		//Button.waitForAnyPress();
    }
	
	private static String determineRecString(byte[] input){
		switch(input[0]){
			case 0: return "A";
			case -3: return "B";
			case -5: return "AB";
			case -7: return "CStart";
			case -9: return "CStop";
			default: 
			//System.out.println("Ups, wrong input: " + (char)input[0]);
			return "Error";
		}
	}
	
	private static void control(String input){
		switch(input){
			case "A":
				backAndForthA();
				break;
			case "B":
				backAndForthB();
				break;
			case "CStart":
				fullSpeedC();
				break;
			case "CStop":
				stopC();
				break;
			case "AB":
				backAndForthAB();
				break;
			default:
				break;
		}
		
		//mc.stop();
		//Button.waitForAnyPress();
	}
	
	private static void backAndForthA(){
		ma.setPower(100);
		int count = 0;
		
		while(count < 2){
			ma.stop();
			if(count % 2 == 0){
				ma.backward();
			}
			else{
				ma.forward();
			}
			Delay.msDelay(180);
			count++;
		}
		
		ma.stop();
		//Power: 50		Delay:380
		//Power: 75		Delay:240
		//Power: 100	Delay:180
	}
	
	private static void backAndForthB(){
		mb.setPower(100);
		int count = 0;
		
		while(count < 2){
			mb.stop();
			if(count % 2 == 0){
				mb.backward();
			}
			else{
				mb.forward();
			}
			Delay.msDelay(180);
			count++;
		}
		
		mb.stop();
	}
	
	private static void backAndForthAB(){
		ma.setPower(100);
		mb.setPower(100);
		int count = 0;
		
		while(count < 2){
			if(count % 2 == 0){
				ma.backward();
				mb.backward();
			}
			else{
				ma.forward();
				mb.forward();
			}
			Delay.msDelay(180);
			count++;
		}
		
		ma.stop();
		mb.stop();
	}
	
	private static void fullSpeedC(){
		mc.setPower(50);
		mc.backward();
	}
	
	private static void stopC(){
		mc.stop();
	}
}