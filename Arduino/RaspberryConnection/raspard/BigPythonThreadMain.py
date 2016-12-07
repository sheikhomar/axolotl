import time
import _thread
import bluetooth


from lib import *

################################################                  #####  #####    #    ###    #####
#											   #                  #        #     # #   #  #     #
#               BIG PYTHON MAIN                #                  #####    #    #####  ###      #
#											   #	                  #    #    #   #  #  #     #
################################################                  #####    #    #   #  #   #    #

####### Serial communication instance    #########
SLib = SerLib()

ser = SLib.make_serial()

#########   Bluetooth stuff  ############
BLib = BlueLib()
s = BLib.setup_server()
clients = [] #Client is initially set to empty and is later appended in thread

def try_bluetooth_send(client_list, message):
	#print('Number of clients: {clientnumber}'.format(clientnumber=len(client_list)))
	for x in range(len(client_list)-1, -1, -1):
		try:
			print('And he tries. Oh my god does he try')
			client_list[x].send(message)
		except bluetooth.BluetoothError:
			print('BluetoothError excepted and client removed')
			client_list.pop(x)
	#print('Number of clients after: {clientnumber}'.format(clientnumber=len(client_list)))
		

###### Threading stuff #######
bluetooth_lock = _thread.allocate_lock()
BTReady = False

bin_buf = []
pack_buf = []
thread_number = 2

def make_client_thread(server_input):
	global BTReady, clients, thread_number #Tell interpreter to use global BTReady instead of creating new local
	print('Thread {number} started'.format(number=thread_number))
	client_local = BLib.make_client(server_input)
	bluetooth_lock.acquire()
	clients.append(client_local)
	for b in bin_buf:
		client_local.send(BLib.bluetooth_format_bin(b))
	for p in pack_buf:
		client_local.send(BLib.bluetooth_format_package(p, pa))
	BTReady = True
	print('Thread {number} is done'.format(number=thread_number))
	bluetooth_lock.release()
	thread_number = thread_number + 1
	_thread.start_new_thread(make_client_thread, (server_input,)) #Start another thread to handle next bluetooth connection



	

	

bin1 = Bin(width=8, length=8, max_layers=2)
pa = PackingAdvisor(bin1)

bin_buf.append(pa.bins[0])
bin_buf.append(pa.bins_foreign[0])

_thread.start_new_thread(make_client_thread, (s,)) #Start new thread to handle accepting bluetooth connection.

data = []

while True: #THE REAL DEAL
	input_serial = SLib.serial_read(ser, data)
	if input_serial[1]:
		print('Message received')
		if input_serial[0][2] == b'p':
			bluetooth_lock.acquire()
			p1 = SLib.format_serial_package(input_serial[0])
			if pa.handle(p1):
				if BTReady:
					try_bluetooth_send(clients, BLib.bluetooth_format_bin(pa.find_bin_containing_package(p1)))
					bin_buf.append(pa.find_bin_containing_package(p1))
				else:
					bin_buf.append(pa.find_bin_containing_package(p1))
			SLib.serial_write_push(ser, pa.find_bin_containing_package(p1).bin_id)
			if BTReady:
				try_bluetooth_send(clients, BLib.bluetooth_format_package(p1, pa))
				pack_buf.append(p1)
			else:
				pack_buf.append(p1)
			bluetooth_lock.release()
	data.clear()
	#time.sleep(1)
