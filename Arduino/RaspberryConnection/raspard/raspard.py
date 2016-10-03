import serial
import time

ser = serial.Serial('/dev/ttyACM0',9600)
data = ser.readline()
print("Message from Arduino {}".format(data))
ser.write("42\n")

while True:
    number = ser.readline()
    print("Received from Arduino {}".format(number))
    new_number = int(number) + 1
    print("Sending to Arduino {}".format(new_number))
    ser.write("{}\n".format(new_number))
    time.sleep(1)
