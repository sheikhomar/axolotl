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
		int succesful = 0, count = 0, countMax = 256;
        int[][] finalBuff = new int[countMax][3];
		byte[] recBuff = new byte[countMax];
		String recString = new String("");
		recBuff[0] = (byte)0;
		
		
		RS485.hsEnable(9600, 0);//57600
		Stopwatch stopwatch = new Stopwatch();
		
		System.out.println("Waiting");
		while(true){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			Delay.msDelay(100);
			if(succesful != 0)
				break;
		}
		System.out.println("Done waiting");
		stopwatch.reset();
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
			if(stopwatch.elapsed() > 10000){
				System.out.println("Count: " + Integer.toString(count));
				System.out.println("To long time");
				Delay.msDelay(1000);
				break;
			}
		}
		System.out.println("Done recieving");
		Delay.msDelay(1000);
		for(int i = 0; i < countMax; i++){
			System.out.println(Integer.toString(finalBuff[i][0]));// + ", " + Integer.toString(finalBuff[i][2]));
			Delay.msDelay(500);
		}
		System.out.println("Done printing");
		
		Button.waitForAnyPress();
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