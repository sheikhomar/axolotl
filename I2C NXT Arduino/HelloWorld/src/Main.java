import lejos.nxt.*;

public class Main {
    static int I2CSlaveAddress = 8;
    //static byte[] buffReadResponse = new byte[8];

    public static void main(String[] args) throws InterruptedException {
        System.out.println("Arduino Connection Test");
        I2CSensor arduino = new I2CSensor(SensorPort.S1, I2CSlaveAddress);
        TouchSensor ts = new TouchSensor(SensorPort.S2);

        int succesful;
        byte[] buf = {0, 5};

        while (true) {
            arduino.sendData(0, buf, buf.length);

            if(ts.isPressed())
            {
                System.out.println("Is pressed");
                succesful = arduino.getData('A', buf, buf.length);
            }
            else
            {
                System.out.println("Not pressed");
                succesful = arduino.getData('S', buf, buf.length);
            }


            //System.out.println("Succesful = " + succesful);
            System.out.println("Size " + buf[0]);
            Thread.sleep(1500);
        }
    }
}