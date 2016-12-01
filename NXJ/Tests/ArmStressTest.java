import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;

public class ArmStressTest {
	private static NXTMotor ma = new NXTMotor(MotorPort.A);
	private static NXTMotor mb = new NXTMotor(MotorPort.B);
	private static NXTMotor mc = new NXTMotor(MotorPort.C);
	private static byte[] recBuff = {0};
	private static final int DATASIZE = 11;
	private static Stopwatch stopwatch = new Stopwatch();
	private static boolean beltRunning = false;

    public static void main(String[] args) throws InterruptedException {
		System.out.println("A-mei-zing!");
		mc.setPower(50);
		mc.stop();
		String recString = new String("");
		
		Button.waitForAnyPress();
		recBuff[0] = 100;
		control("CSpeed");
		control("CStart");
		
		while(!Button.ESCAPE.isPressed()){
			control("A");
		}
		
		control("CStop");
		System.out.println("Job's Done");
		Button.waitForAnyPress();
    }
	
	private static void control(String input){ //Running correct function
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
			case "CSpeed":
				adjustSpeedC();
				break;
			default:
				break;
		}
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
	
	private static void fullSpeedC(){
		mc.forward();
		beltRunning = true;
	}
	
	private static void stopC(){
		mc.stop();
		beltRunning = false;
	}
	
	private static void adjustSpeedC(){
		if(0 <= recBuff[0] && recBuff[0] <= 100){
			mc.setPower(recBuff[0]);
			System.out.println("Speed is now " + Byte.toString(recBuff[0]));
		}
		else{
			System.out.println(Byte.toString(recBuff[0]) + " is no speed");
		}
	}
	
	private static void buttonFuncs(){
		if(Button.ESCAPE.isPressed()){
			mc.stop();
			beltRunning = false;
			Delay.msDelay(500);
		}
		else if(Button.LEFT.isPressed()){
			System.out.println("Godt arbejde! /S");
			Delay.msDelay(500);
		}
		else if(Button.RIGHT.isPressed()){
			System.out.println("Godt arbejde!");
			Delay.msDelay(500);
		}
		else if(Button.ENTER.isPressed()){
			if(beltRunning){
				mc.backward();
				beltRunning = true;
			}
			else{
				mc.forward();
				beltRunning = true;
			}
			Delay.msDelay(500);
		}
	}
}