import time

from lib import *

SLib = SerLib()
ser = SLib.make_serial()

data = []
bin1 = Bin(width=5, length=10, max_layers=1)
pa = PackingAdvisor(bin1)

while True:
	cyka = SLib.serial_read(ser, data)
	if cyka[1]:
		print('Message received')
		if cyka[0][2] == b'p':
			p1 = SLib.format_serial_package(cyka[0])
			print(p1.colour)
			print(p1.length)
			print(p1.width)
			pa.handle(p1)
			print(((pa.find_bin_containing_package(p1).bin_id % 2) + 1).to_bytes(1, byteorder='little'))
			serial_write_push(ser, pa.find_bin_containing_package(p1).bin_id)
		elif cyka[0][2] == b'R':
			SLib.serial_write_last_push(ser)
	data.clear()
	time.sleep(1)
	
	#serial_write_push(ser, 1)
	#time.sleep(2)
	
	#print('Starting write')
	#ser.write(b'a')
	#ser.write((1).to_bytes(1, byteorder='little'))
	#ser.write(b'p')
	#ser.write((2).to_bytes(1, byteorder='little'))
	#print('Finished write')