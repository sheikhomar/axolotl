import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;

public class ControlBrick {
	private static NXTMotor ma = new NXTMotor(MotorPort.A);
	private static NXTMotor mb = new NXTMotor(MotorPort.B);
	private static NXTMotor mc = new NXTMotor(MotorPort.C);

    public static void main(String[] args) throws InterruptedException {
		LCD.drawString("A-mei-zing!",3,3);
		int succesful = 0;
		byte[] recBuff = new byte[1];
		String recString = new String("");
		recBuff[0] = (byte)0;
		
		RS485.hsEnable(57600, 0);
		
		while(true){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			if(succesful != 0){
				recString = determineRecString(recBuff);
				LCD.clear();
				System.out.println("Running " + recString);
				control(recString);
			}
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
	
	private static String determineRecString(byte[] input){
		switch(input[0]){
			case 0: return "A";
			case 1: return "B";
			case 2: return "AB";
			case 3: return "CStart";
			case 4: return "CStop";
			case 5: return "Colour";
			default: return "Error";
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
			case "Colour":
				readAndSendColour();
				break;
			default:
				break;
		}
	}
	
	private static void readAndSendColour(){
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int succesful = 0, colour = -1;
		byte[] sendBuff = new byte[1];
		sendBuff[0] = (byte)-1;
		
		System.out.println("Scanning colour");
		colour = sensor.getColorID();
		sendBuff[0] = (byte)colour;
		System.out.println("Sending " + ConvertColorEnum(colour));
		while(succesful == 0){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length);
		}
		System.out.println("Colour send");
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