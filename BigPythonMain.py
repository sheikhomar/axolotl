import time


from lib import *


####### Serial communication instance    #########
SLib = SerLib()

ser = SLib.make_serial()
	
################################################                  #####  #####    #    ###    #####
#											   #                  #        #     # #   #  #     #
#               BIG PYTHON MAIN                #                  #####    #    #####  ###      #
#											   #	                  #    #    #   #  #  #     #
################################################                  #####    #    #   #  #   #    #

#########   Bluetooth stuff  ############
BLib = BlueLib()
s = BLib.setup_server()
client = BLib.make_client(s)	
	

	

bin1 = Bin(width=5, length=10, max_layers=3)
pa = PackingAdvisor(bin1)

client.send(BLib.bluetooth_format_bin(pa.bins[0]))
client.send(BLib.bluetooth_format_bin(pa.bins_foreign[0]))

data = []

while True:
	input_serial = SLib.serial_read(ser, data)
	if input_serial[1]:
		print('Message received')
		if input_serial[0][2] == b'p':
			p1 = SLib.format_serial_package(input_serial[0])
			if pa.handle(p1):
				client.send(BLib.bluetooth_format_bin(pa.find_bin_containing_package(p1)))
			print(((pa.find_bin_containing_package(p1).bin_id % 2) + 1).to_bytes(1, byteorder='little'))
			SLib.serial_write_push(ser, pa.find_bin_containing_package(p1).bin_id)
			client.send(BLib.bluetooth_format_package(p1, pa))
		elif input_serial[0][2] == b'R':
			SLib.serial_write_last_push(ser)
	data.clear()
	time.sleep(1)
