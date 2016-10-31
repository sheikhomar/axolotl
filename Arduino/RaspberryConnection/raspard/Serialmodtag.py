import serial
import time

ser = serial.Serial('/dev/ttyACM0',9600)
data = []

def read_x_bytes(ser, data, x):
	i = 0
	while True:
		if ser.inWaiting() >= 1:
			data.append(ser.read())
			i = i + 1
			if i == x:
				return data

def serial_read(ser, data):	
	#Simulated Do-while loop that reads over initial noise
	while True:
		if ser.inWaiting() >= 1:
			data.append(ser.read())
			if data[0] == b'r' or data[0] == b'n' or data[0] == b'a':
				print('YES')
				break
			else:
				print('FUCK')
				data.clear()
	
	#If message is not for raspberry PI, skip over it		
	if data[0] == b'a' or data[0] == b'n':
		while True:
			if ser.inWaiting() >= 1:
				data.append(int.from_bytes(ser.read(), byteorder='little'))
				if data[1] <= 11:
					data = read_x_bytes(ser, data[1])
					data.clear()
					return serial_read(ser, read)
	
	if data[0] == b'r':
		while True:
			if ser.inWaiting() >= 1:
				data.append(int.from_bytes(ser.read(), byteorder='little'))
				if data[1] <= 11:
					data = read_x_bytes(ser, data[1])
					return data
	else:
		raise ArithmeticError('First byte does not indicate proper message')
	
	
serial_read(ser, data)

