import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;

public class ColourTestRapp {
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
		recBuff[0] = 50;
		control("CSpeed");
		control("CStart");
		readAndPrintBrickCoulour(20);
		recBuff[0] = 75;
		control("CSpeed");
		readAndPrintBrickCoulour(20);
		recBuff[0] = 100;
		control("CSpeed");
		readAndPrintBrickCoulour(20);
		control("CStop");
		System.out.println("Job's Done");
		Button.waitForAnyPress();
    }
	
	private static void control(String input){ //Running correct function
		switch(input){
			case "CStart":
				fullSpeedC();
				break;
			case "CStop":
				stopC();
				break;
			case "Colour":
				readAndSendColour();
				break;
			case "CSpeed":
				adjustSpeedC();
				break;
			default:
				break;
		}
	}
	
	private static String ConvertColorEnum(int input) {
		switch(input) {
		case 0: return "Red";
		case 1: return "Green";
		case 2: return "Blue"; 
		case 3: return "Yellow";
		case 6: return "White";
		case 7: return "Black";
		default: return "Irrelevant";
		}
	}
	
	private static void readAndSendColour(){ //Reading colour and sending it to Ardu no matter the result
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int succesful = 0, colour = -1;
		byte[] sendBuff = new byte[4];
		sendBuff[0] = 'a';
		sendBuff[1] = 1;
		sendBuff[2] = 1;
		
		colour = sensor.getColorID();
		sendBuff[3] = (byte)colour;
		while(succesful == 0){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length); //hsWrite --> Returning number of bytes send
		}
		System.out.println("Sending " + ConvertColorEnum(colour));
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
	
	private static void readAndPrintBrickCoulour(int numbOfBricksToRead){
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int succesful = 0, colour = -1;
		
		for(int i = 0; i < numbOfBricksToRead; i++){
			while (!(colour == 0 || colour == 1 || colour == 2 || colour == 3)) { //0 = Red, 1 = Green, 2 = Blue, 3 = Yellow
				colour = sensor.getColorID();
			}
			System.out.println(i + ": " + ConvertColorEnum(colour));
			colour = -1;
			Delay.msDelay(1000);
		}
	}
}