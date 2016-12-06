import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;
import java.util.*;

public class ColourReadCount {
	private static NXTMotor ma = new NXTMotor(MotorPort.A);
	private static NXTMotor mb = new NXTMotor(MotorPort.B);
	private static NXTMotor mc = new NXTMotor(MotorPort.C);
	private static byte[] recBuff = {0};
	private static final int DATASIZE = 11;
	private static Stopwatch stopwatch = new Stopwatch();
	private static boolean beltRunning = false;
	private static List<Integer> colourList = new ArrayList<Integer>();
	private static boolean readyForNewBlock = true;
	private static int badColourCount = 0;
	private static int goodColourCount = 0;
	private static int previousColour = 128;

    public static void main(String[] args) throws InterruptedException {
		System.out.println("A-mei-zing!");
		mc.setPower(50);
		mc.stop();
		String recString = new String("");
		
		control("CStart");
		
		while(true){
			colourReadCount();
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
		case 128: return "No Colour";
		default: return "Irrelevant";
		}
	}
	
	private static String determineRecString(){
		readInput(); //Reading and discarding length of message/input
		if(recBuff[0] < DATASIZE + 1){ //OBS. See contract if confused -- length should be legal
			readInput();
			switch(recBuff[0]){ //Function to be performed
				case 0: return "A";
				case 1: return "B";
				case 2: return "AB";
				case 3: return "CStart";
				case 4: return "CStop";
				case 5: return "SendColour";
				case 6: return "CSpeed";
				case 11: return "ColourTest";
				default: return "unknown program";
			}
		}
		else{
			return "nothing";
		}
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
			case "AB":
				backAndForthAB();
				break;
			case "ReadColour":
				readColour();
				break;
			case "SendColour":
				sendColour();
				break;
			case "CSpeed":
				adjustSpeedC();
				break;
			case "ColourTest":
				readAndSendBrickColour();
				break;
			default:
				break;
		}
	}
	
	private static void colourReadCount(){ //How many times can the colour sensor read on one block?
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int colour = -1, goodReadCount = 0;
		boolean readingBlock = false;
		
		while(readyForNewBlock){
			colour = sensor.getColorID();
			Delay.msDelay(10);
			if(colour == 0 || colour == 1 || colour == 2 || colour == 3){ //Check if read colour is red, green, blue or yellow
				readyForNewBlock = false;
				readingBlock = true;
			}
			buttonFuncs();
		}
		while(readingBlock){
			colour = sensor.getColorID();
			Delay.msDelay(10);
			if(colour == 0 || colour == 1 || colour == 2 || colour == 3){ //Check if read colour is red, green, blue or yellow
				goodReadCount++;
			}
			else{
				readingBlock = false;
				readyForNewBlock = true;
				break;
			}
		}
		
		System.out.println("Reads: " + goodReadCount);
	}
	
	private static void readColour(){ //Reading colour
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int colour = -1;

		colour = sensor.getColorID();
		if(readyForNewBlock){
			if(colour == 0 || colour == 1 || colour == 2 || colour == 3){ //Check if read colour is red, green, blue or yellow
				if(previousColour == 128){
					previousColour = colour;
				}
				else if(previousColour == colour){
					goodColourCount++;
				}
				else{
					goodColourCount = 0;	
				}
				if(goodColourCount >= 3){
					colourList.add(colour);
					readyForNewBlock = false;
					badColourCount = 0;
					goodColourCount = 128;
				}
			}
		}
		else if(colour == 6 || colour == 7){
			if(badColourCount >= 3){
				readyForNewBlock = true;
				System.out.println("Ready");
			}
			badColourCount++;
		}
	}
	
	private static void sendColour(){ //Sending colour to Ardu if a valid colour have been read
		int succesful = 0, tempColour = 0;
		byte[] sendBuff = new byte[4];
		sendBuff[0] = 'a';
		sendBuff[1] = 1;
		sendBuff[2] = 1;
		
		if(!colourList.isEmpty()){
			tempColour = colourList.remove(0);
			sendBuff[3] = (byte)tempColour;
		}
		else{
			sendBuff[3] = (byte)128;
		}
		
		while(succesful == 0){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length); //hsWrite --> Returning number of bytes send
		}
		if(sendBuff[3] == 0 || sendBuff[3] == 1 || sendBuff[3] == 2 || sendBuff[3] == 3){
			//control("CStop");
			System.out.println("List: " + colourList.size());
			//Delay.msDelay(3000);
			//control("CStart");
		}
		
		System.out.println("Sending " + ConvertColorEnum(((int)sendBuff[3])));
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
		mc.forward();
		beltRunning = true;
	}
	
	private static void stopC(){
		mc.stop();
		beltRunning = false;
	}
	
	private static void adjustSpeedC(){
		readInput();
		if(0 <= recBuff[0] && recBuff[0] <= 100){
			mc.setPower(recBuff[0]);
			System.out.println("Speed is now " + Byte.toString(recBuff[0]));
		}
		else{
			System.out.println(Byte.toString(recBuff[0]) + " is no speed");
		}
	}
	
	private static void readInput(){ //When calling readInput the default is no parameter which result in no timeout
		readInput(false);
	}
	
	private static void readInput(boolean timeLimitActive){ //When calling readInput(para) the default is no msg when timeouting
		readInput(timeLimitActive, false);
	}
	
	private static void readInput(boolean timeLimitActive, boolean enableMsg){ //Reading input untill more than 0 bytes are read
		int succesful = 0;
		while(succesful == 0){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			if(timeLimitActive && maxReadingTimeReached(enableMsg)){
				break;
			}
		}
	}
	
	private static boolean maxReadingTimeReached(boolean enableMsg){ //Timeout when reading inputs
		final int MAX_READING_TIME = 2;
		if(stopwatch.elapsed() > MAX_READING_TIME){
			if(enableMsg){
				System.out.println("Max reading time reached");
			}
			return true;
		}
		else{
			return false;
		}
	}
	
	private static void skipInput(){ //Skipping irrelevant inputs
		int inputToSkip = 0;
		readInput();
		inputToSkip = recBuff[0] + 1;
		if(inputToSkip < DATASIZE + 1){ //OBS. See contract if confused
			stopwatch.reset();
			for(int i = 0; i < inputToSkip; i++){ //Skipping the data send
				readInput(true, true);
			}
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
	
	private static void readAndSendBrickColour(){ // For test purpose only, reads colour untill a valid brick colour is read -- can livelock system
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int succesful = 0, colour = -1;
		byte[] sendBuff = new byte[4];
		sendBuff[0] = 'a';
		sendBuff[1] = 1;
		sendBuff[2] = 1;
		
		while (!(colour == 0 || colour == 1 || colour == 2 || colour == 3)) { //0 = Red, 1 = Green, 2 = Blue, 3 = Yellow
			colour = sensor.getColorID();
		}
		sendBuff[3] = (byte)colour;
		while(succesful == 0){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length); //Sending untill 0 < bytes are send
		}
		System.out.println("Sending " + ConvertColorEnum(colour));
	}
}