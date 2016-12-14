import time

from lib import *

SLib = SerLib()
ser = SLib.make_serial()

data = []

while True:
	cyka = SLib.serial_read(ser, data)
	if cyka[1]: 
		print('Message received')
		if cyka[0][2] == b't':
			print('Received {letter}'.format(letter=cyka[0][3]))
			print('Sending {letter}'.format(letter=bytes(cyka[0][3]).swapcase()))
			ser.write(bytes.fromhex('610174{hex}'.format(hex=hex(int.from_bytes(cyka[0][3].swapcase(), byteorder='little')).replace('0x', '')))) #61=a 01 = 1 74 = t: Hardcoded message for performance
	data.clear()
	time.sleep(1)
