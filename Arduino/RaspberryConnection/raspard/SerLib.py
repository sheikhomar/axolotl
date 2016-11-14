import serial

class SerLib:
	def __init__(self, baudrate=57600):
		self.baudrate = baudrate

	def format_serial_package(self, input_array):
		if len(input_array) == 7:
			return Package(width=int.from_bytes(input_array[4], byteorder='little'), length=int.from_bytes(input_array[3], byteorder='little'), colour=int.from_bytes(input_array[6], byteorder='little'))
		else:
			raise InvalidArgError('Input to format_serial_package was not array of length 7')
	
	def read_x_bytes(self, ser, data, x):
		i = 0
		while True:
			if ser.inWaiting() >= 1:
				data.append(ser.read())
				i = i + 1
				if i == x:
					return data
	
	def serial_read(self, ser, data):	
		#Simulated Do-while loop that reads over initial noise
		while True:
			if ser.inWaiting() >= 3:
				data.append(ser.read())
				if data[0] == b'r' or data[0] == b'n' or data[0] == b'a':
					print('Valid start char accepted')
					break
				else:
					print('waiting: ' + str(ser.inWaiting()) + '      just read: ' + str(data[0]))
					data.clear()
			else:
				#print('Not a full message in waiting')
				return (data, False)
		
		
		#If message is not for raspberry PI, skip over it		
		if data[0] == b'a' or data[0] == b'n':
			while True:
				if ser.inWaiting() >= 1:
					data.append(int.from_bytes(ser.read(), byteorder='little'))
					if data[1] <= 11:
						data = self.read_x_bytes(ser, data, x = data[1] + 1)
						data.clear()
						return self.serial_read(ser, data)
					else:
						print('Implied message for Non-Raspberry, but length was incompatible: {}'.format(data[1]))
						data.clear()
						return self.serial_read(ser, data)
		
		if data[0] == b'r':
			while True:
				if ser.inWaiting() >= 1:
					data.append(int.from_bytes(ser.read(), byteorder='little'))
					if data[1] <= 11:
						data = self.read_x_bytes(ser, data, x=data[1] + 1)
						return (data, True)
					else:
						print('Implied message for Raspberry, but length was incompatible: {}'.format(data[1]))
						data.clear()
						return self.serial_read(ser, data)
		else:
			raise ArithmeticError('First byte does not indicate proper message')
			
	def serial_write_push(self, ser, bin_id):
		ser.write(b'a')
		ser.write((1).to_bytes(1, byteorder='little'))
		ser.write(b'p')
		ser.write(((bin_id % 2) + 1).to_bytes(1, byteorder='little'))
		print('Just send bin_id: {bin_id}'.format(bin_id=bin_id))
		
	def make_serial(self):
		serial_instance = serial.Serial('/dev/ttyUSB0', self.baudrate)
		return serial_instance
