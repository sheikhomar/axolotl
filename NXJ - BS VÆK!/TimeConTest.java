import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;

public class TimeConTest {
	private static NXTMotor ma = new NXTMotor(MotorPort.A);
	private static NXTMotor mb = new NXTMotor(MotorPort.B);
	private static NXTMotor mc = new NXTMotor(MotorPort.C);
    static String RS485Address = "8";

    public static void main(String[] args) throws InterruptedException {
        //System.out.println("Arduino Connection Test");
		//LCD.drawString("A-mei-zing!",3,3);
		int succesful = 0, count = 0, countMax = 26;
        int[][] finalBuff = new int[countMax][3];
		byte[] recBuff = new byte[countMax];
		String recString = new String("");
		recBuff[0] = (byte)0;
		
		
		RS485.hsEnable(57600, 0);//57600
		Stopwatch stopwatch = new Stopwatch();
		stopwatch.reset();
		
		System.out.println("Waiting");
		while(true){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			//Delay.msDelay(100);
			if(succesful != 0)
				break;
		}
		System.out.println("Done waiting");
		while(count < countMax){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			if(count != 0)
				if(finalBuff[count-1][0] != recBuff[0]){
					finalBuff[count][0] = recBuff[0];
					finalBuff[count][1] = stopwatch.elapsed();
					finalBuff[count][2] = finalBuff[count][1] - finalBuff[count-1][1];
				}
				else{
					count--;
				}
			else{
				finalBuff[count][0] = recBuff[0];
				finalBuff[count][1] = stopwatch.elapsed();
				finalBuff[count][2] = 0;
			}
			count++;
			//if(stopwatch.elapsed()-finalBuff[0][1] > 15000){
			//	System.out.println("Count: " + Integer.toString(count));
			//	System.out.println("To long time");
			//	Delay.msDelay(1000);
			//	break;
			//}
		}
		System.out.println("Done recieving");
		Delay.msDelay(1000);
		for(int i = 0; i < countMax; i++){
			System.out.println(decideChar(finalBuff[i][0]));// + ", " + Integer.toString(finalBuff[i][2]));
			Delay.msDelay(500);
		}
		System.out.println("Done printing");
		
		Button.waitForAnyPress();
    }
	
	private static String decideChar(int input){
		switch(input){
			case 97: return "a";
			case 98: return "b";
			case 99: return "c";
			case 100: return "d";
			case 101: return "e";
			case 102: return "f";
			case 103: return "g";
			case 104: return "h";
			case 105: return "i";
			case 106: return "j";
			case 107: return "k";
			case 108: return "l";
			case 109: return "m";
			case 110: return "n";
			case 111: return "o";
			case 112: return "p";
			case 113: return "q";
			case 114: return "r";
			case 115: return "s";
			case 116: return "t";
			case 117: return "u";
			case 118: return "v";
			case 119: return "w";
			case 120: return "x";
			case 121: return "y";
			case 122: return "z";
			default: return "Invalid";
		}
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