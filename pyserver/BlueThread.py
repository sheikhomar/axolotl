import time
import logging
import bluetooth
import _thread
import random

from lib import *

#########   Bluetooth stuff  ############
BLib = BlueLib()
	
def blue_test(server_input):
	global BTReady, client, bin_buf_lock #Tell interpreter to use global BTReady instead of create local var
	print('Thread 2 started')
	client = BLib.make_client(server_input)
	bin_buf_lock.acquire()
	for b in bin_buf:
		client.send(BLib.bluetooth_format_bin(b))
	for p in pack_buf:
		client.send(BLib.bluetooth_format_package(p, pa))
	BTReady = True
	print('Thread 2 is done')
	bin_buf_lock.release()
	
################# BEGIN MAIN ##########

server = BLib.setup_server()
client = None
BTReady = False
bin_buf_lock = _thread.allocate_lock()

bin1 = Bin(width=5, length=10, max_layers=3)
pa = PackingAdvisor(bin1)
bin_buf = []
pack_buf = []

bin_buf.append(pa.bins[0])
bin_buf.append(pa.bins_foreign[0])

_thread.start_new_thread(blue_test, (server,))

###Random colour code from internet###
weighted_choices = [(0, 5), (2, 2), (3, 2), (1, 1)]
population = [val for val, cnt in weighted_choices for i in range(cnt)]


while(True):

	package = Package(width=random.randrange(4)+1, length=2, colour=random.choice(population)) #random.randrange(4))

	new_bin = pa.handle(package)
	
	bin_buf_lock.acquire()
	if BTReady:
		print('Hillary is not jail anyway')
		if new_bin:
			client.send(BLib.bluetooth_format_bin(pa.find_bin_containing_package(package)))
		client.send(BLib.bluetooth_format_package(package, pa))
	else:
		print('Trump is great')
		if new_bin:
			bin_buf.append(pa.find_bin_containing_package(package))
		pack_buf.append(package)
	bin_buf_lock.release()
	time.sleep(2)