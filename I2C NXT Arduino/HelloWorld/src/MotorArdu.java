import lejos.nxt.*;
import lejos.util.Delay;

public class MotorArdu {
	private static NXTMotor ma = new NXTMotor(MotorPort.A);
	private static NXTMotor mb = new NXTMotor(MotorPort.B);
	private static NXTMotor mc = new NXTMotor(MotorPort.C);
    static int I2CSlaveAddress = 8;

    public static void main(String[] args) throws InterruptedException {
        System.out.println("Arduino Connection Test");
		LCD.drawString("A-mei-zing!",3,3);
        I2CSensor arduino = new I2CSensor(SensorPort.S1, I2CSlaveAddress);
        byte[] sendBuff = new byte[2];
		byte[] recBuff = new byte[2];
		int succesful = 0;
		String recString = new String("");
		sendBuff[0] = (byte)0;
		recBuff[0] = (byte)0;
		
		//arduino.sendData(0, sendBuff, sendBuff.length);
        
        //succesful = arduino.getData('S', recBuff, recBuff.length);
		
		
		String[] testInput = {"A"};
		control(testInput);
		
    }
	
	private static String determineRecString(byte[] input){
		switch(input[0]){
			case 0: return "A";
			case 1: return "B";
			case 2: return "AB";
			case 3: return "CStart";
			case 4: return "CStop";
			default: 
			System.out.println("Ups, wrong input: " + (char)input[0]);
			break;
		}
		return null;
	}
	
	private static void control(String[] input){
		for(int i = 0; i < input.length; i++){
			switch(input[i]){
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
		}
		
		mc.stop();
		Button.waitForAnyPress();
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
		mc.setPower(100);
		mc.backward();
	}
	
	private static void stopC(){
		mc.stop();
	}
}