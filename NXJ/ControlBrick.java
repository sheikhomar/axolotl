import lejos.nxt.*;
import lejos.util.*;
import lejos.nxt.comm.RS485;
import java.util.*;

public class ControlBrick {	
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
	private static int previousColour = 128;
	private static int goodColourCount = 0;
	private static int listResetCount = 0;
	private static final int RED = 0, GREEN = 1, BLUE = 2, YELLOW = 3, WHITE = 6, BLACK = 7, NOCOLOUR = 127;
	private static final String REDSTRING = "Red", GREENSTRING = "Green", BLUESTRING = "Blue", YELLOWSTRING = "Yellow", 
								WHITESTRING = "White", BLACKSTRING = "Black", NOCOLOURSTRING = "NoColour";
	private static final String AARM = "A", BARM = "B", CBELTSTART = "CStart", CBELTSTOP = "CStop", CBELTSPEED = "CSpeed", 
								CSENSORREAD = "ReadColour", CSENSORSEND = "SendColour", CSENSORTEST = "ColourTest", DEFAULTPROGRAM = "unknown program";
	private static final int MIN_GOODCOLOURCOUNT = 2, MIN_BADCOLOURCOUNT = 5;

    public static void main(String[] args) throws InterruptedException {
		System.out.println("A-mei-zing!");
		mc.setPower(50);
		mc.stop();
		String recString = new String("");
		
		RS485.hsEnable(57600, 0);
		
		while(true){
			buttonFuncs();
			control(CSENSORREAD);
			readInput(true);
			if(recBuff[0] == 110){ //Checking if NXJ should do something or not -- 110 = 'n'
				recString = determineRecString();
				System.out.println("Running " + recString);
				control(recString);
			}
			else if(recBuff[0] == 97 || recBuff[0] == 114 || recBuff[0] == 32){ //97 = 'a', 114 = 'r', 32 = ' '
				skipInput();
			}
			checkToClearColourList();
		}
    }
	
	private static String ConvertColorEnum(int input) {
		switch(input) {
		case RED: return REDSTRING;
		case GREEN: return GREENSTRING;
		case BLUE: return BLUESTRING; 
		case YELLOW: return YELLOWSTRING;
		case WHITE: return WHITESTRING;
		case BLACK: return BLACKSTRING;
		case NOCOLOUR: return NOCOLOURSTRING;
		default: return "Irrelevant";
		}
	}
	
	private static String determineRecString(){
		readInput(); //Reading and discarding length of message/input
		if(recBuff[0] < DATASIZE + 1){ //OBS. See contract if confused -- length should be legal
			readInput();
			switch(recBuff[0]){ //Function to be performed
				case 0: return AARM;
				case 1: return BARM;
				case 3: return CBELTSTART;
				case 4: return CBELTSTOP;
				case 5: return CSENSORSEND;
				case 6: return CBELTSPEED;
				case 11: return CSENSORTEST;
				default: return DEFAULTPROGRAM;
			}
		}
		else{
			return "nothing";
		}
	}
	
	private static void control(String input){ //Running correct function
		switch(input){
			case AARM:
				backAndForthA();
				break;
			case BARM:
				backAndForthB();
				break;
			case CBELTSTART:
				fullSpeedC();
				break;
			case CBELTSTOP:
				stopC();
				break;
			case CSENSORREAD:
				readColour();
				break;
			case CSENSORSEND:
				sendColour();
				break;
			case CBELTSPEED:
				adjustSpeedC();
				break;
			case CSENSORTEST:
				readAndSendBrickColour();
				break;
			default:
				break;
		}
	}
	
	private static void readColour(){ //Reading colour
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int colour = NOCOLOUR;

		colour = sensor.getColorID();
		Delay.msDelay(7); //Can't read colour sensor to often so we need a small delay
		if(readyForNewBlock){
			if(colour == RED || colour == GREEN || colour == BLUE || colour == YELLOW){ //Check if read colour is valid
				if(previousColour == colour){ //previousColour is NOCOLOUR as default -- here we're checking if previous colour read is the same as the current read colour
					goodColourCount++;
				}
				else{ //If we just read another valid colour than we should e.g. the brick is blue and we read a green instead
					previousColour = colour;
					goodColourCount = 0;
				}
				
				if(goodColourCount >= MIN_GOODCOLOURCOUNT){ //We need to read the same colour MIN_GOODCOLOURCOUNT(2) in a row before we conclude it's the true colour of the brick
					colourList.add(colour);
					readyForNewBlock = false;
					badColourCount = 0;
					previousColour = NOCOLOUR;
					goodColourCount = 0;
				}
				
				listResetCount = stopwatch.elapsed();
			}
		}
		else if(colour == WHITE || colour == BLACK){ //We want to read at least MIN_BADCOLOURCOUNT(5) black or white colours between each brick before we conclude we are ready for the next brick
			if(badColourCount >= MIN_BADCOLOURCOUNT){
				readyForNewBlock = true;
			}
			badColourCount++;
		}
	}
	
	private static void sendColour(){ //Sending colour to Ardu
		int succesful = 0, tempColour = NOCOLOUR;
		byte[] sendBuff = new byte[4];
		sendBuff[0] = 'a';
		sendBuff[1] = 1;
		sendBuff[2] = 1;
		
		if(!colourList.isEmpty()){
			tempColour = colourList.remove(0);
			sendBuff[3] = (byte)tempColour;
		}
		else{
			sendBuff[3] = (byte)NOCOLOUR; //Default colour send to arduino if the list is empty (the package is removed from arudino)
		}
		
		while(succesful == 0){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length); //hsWrite --> Returning number of bytes send
		}
		
		System.out.println("Sending " + ConvertColorEnum(((int)sendBuff[3])));
	}
	
	private static void backAndForthA(){
		executeOnePush(ma); //Running in a thread making the program able to do other stuff (there's two delays for 180 each making this preferable)
	}
	
	private static void backAndForthB(){
		executeOnePush(mb);
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
	
	private static void readInput(){ //When calling readInput the default is no parameter which result in no timeout and no msg
		readInput(false);
	}
	
	private static void readInput(boolean timeLimitActive){ //When calling readInput(para) the default is no msg when timeouting
		readInput(timeLimitActive, false);
	}
	
	private static void readInput(boolean timeLimitActive, boolean enableMsg){ //Reading input untill more than 0 bytes are read
		int succesful = 0;
		while(succesful == 0){
			succesful = RS485.hsRead(recBuff, 0, recBuff.length);
			if(timeLimitActive && maxReadingTimeReached(enableMsg)){ //maxReadingTimeReached is only called when skipInput() is called -- shortcircuit
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
	
	private static void checkToClearColourList(){ //Clear the colourList if more than 1 sec since last brick is read
		final int MAX_READING_TIME = 1000;
		if(stopwatch.elapsed() - listResetCount > MAX_READING_TIME){
			colourList.clear();
			listResetCount = stopwatch.elapsed();
		}
	}
	
	private static void readAndSendBrickColour(){ // For test purpose only, reads colour untill a valid brick colour is read -- can livelock system
		ColorSensor sensor = new ColorSensor(SensorPort.S1);
		int succesful = 0, colour = NOCOLOUR;
		byte[] sendBuff = new byte[4];
		sendBuff[0] = 'a';
		sendBuff[1] = 1;
		sendBuff[2] = 1;
		
		while (!(colour == RED || colour == GREEN || colour == BLUE || colour == YELLOW)) { //0 = Red, 1 = Green, 2 = Blue, 3 = Yellow
			colour = sensor.getColorID();
		}
		sendBuff[3] = (byte)colour;
		while(succesful == 0){
			succesful = RS485.hsWrite(sendBuff, 0, sendBuff.length); //Sending untill 0 < bytes are send
		}
		System.out.println("Sending " + ConvertColorEnum(colour));
	}
	
	private static void executeOnePush(NXTMotor m){ //Running thread where a given motor is performing a push to avoid stalling the system in the delays
		Thread thread = new Thread(){
			public void run(){
				m.setPower(100);
				int count = 0;
				
				while(count < 2){
					m.stop();
					if(count % 2 == 0){
						m.backward();
					}
					else{
						m.forward();
					}
					Delay.msDelay(180);
					count++;
				}
				
				m.stop();
				//Power: 50		Delay:360
				//Power: 75		Delay:240
				//Power: 100	Delay:180
			}
		};
		thread.start();
	}
}