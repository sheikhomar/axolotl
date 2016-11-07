import serial
import time

from lib import *

def format_serial_package(input_array):
    if len(input_array) == 7:
        return Package(int.from_bytes(input_array[4], byteorder='little'), int.from_bytes(input_array[3], byteorder='little'), int.from_bytes(input_array[6], byteorder='little'))
    else:
        raise InvalidArgError('Input to format_serial_package was not array of length 7')

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
		if ser.inWaiting() >= 3:
			data.append(ser.read())
			if data[0] == b'r' or data[0] == b'n' or data[0] == b'a':
				print('YES')
				break
			else:
				print('waiting: ' + str(ser.inWaiting()) + '      just read: ' + str(data[0]))
				data.clear()
		else:
			print('Not a full message in waiting')
			return (data, False)
	
	
	#If message is not for raspberry PI, skip over it		
	if data[0] == b'a' or data[0] == b'n':
		while True:
			if ser.inWaiting() >= 1:
				data.append(int.from_bytes(ser.read(), byteorder='little'))
				if data[1] <= 11:
					data = read_x_bytes(ser, data, x = data[1] + 1)
					data.clear()
					return serial_read(ser, data)
				else:
					print('Implied message for Non-Raspberry, but length was incompatible: {}'.format(data[1]))
					data.clear()
					return serial_read(ser, data)
	
	if data[0] == b'r':
		while True:
			if ser.inWaiting() >= 1:
				data.append(int.from_bytes(ser.read(), byteorder='little'))
				if data[1] <= 11:
					data = read_x_bytes(ser, data, x=data[1] + 1)
					return (data, True)
				else:
					print('Implied message for Raspberry, but length was incompatible: {}'.format(data[1]))
					data.clear()
					return serial_read(ser, data)
	else:
		raise ArithmeticError('First byte does not indicate proper message')
		
def serial_write_push(ser, bin_id):
	ser.write(b'a')
	ser.write((1).to_bytes(1, byteorder='little'))
	ser.write(b'p')
	ser.write(((bin_id % 2) + 1).to_bytes(1, byteorder='little'))
	

ser = serial.Serial('/dev/ttyUSB0',57600)
data = []
bin1 = Bin(width=5, length=10, max_layers=3)
pa = PackingAdvisor(bin1)

while True:
	#cyka = serial_read(ser, data)
	#if cyka[1]:
	#	print('Package received')
	#	p1 = format_serial_package(cyka[0])
	#	print(str(cyka[0][3]))
	#	print(str(cyka[0][4]))
	#	print(str(cyka[0][5]))
	#	print(str(cyka[0][6]))
	#	print(p1.colour)
	#	print(p1.length)
	#	print(p1.width)
	#	pa.handle(p1)
	#	print(((pa.find_bin_containing_package(p1).bin_id % 2) + 1).to_bytes(1, byteorder='little'))
	#	serial_write_push(ser, pa.find_bin_containing_package(p1).bin_id)
	#data.clear()
	#time.sleep(1)
	
	serial_write_push(ser, 1)
	time.sleep(2)
	
	#print('Starting write')
	#ser.write(b'a')
	#ser.write((1).to_bytes(1, byteorder='little'))
	#ser.write(b'p')
	#ser.write((2).to_bytes(1, byteorder='little'))
	#print('Finished write')