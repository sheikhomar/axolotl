import time
import logging
import bluetooth
import _thread
import random

from lib import *

#########   Bluetooth stuff  ############
bluetooth_scale = 1

log = logging.getLogger(__name__)

# The MAC address of a Bluetooth adapter on the server.
# The server might have multiple Bluetooth adapters.
MAC_ADDR = '00:0C:78:33:A5:63'
PORT = 0 #
BACKLOG = 3
BUFFER_SIZE = 1024
SERVICE_ID = "c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd"

def bluetooth_format_package(package, p_advisor):
    fragile = 0

    if package.is_fragile:
        fragile = 1
    if package.position is not None:
        return 'P: {x} {y} {length} {width} {height} {colour} {fragile} {layer} {bin_id}\n'.format(x=package.position.x*bluetooth_scale, y=package.position.y*bluetooth_scale, length=package.length*bluetooth_scale, width=package.width*bluetooth_scale, height=package.height*bluetooth_scale, colour=package.colour, fragile=fragile, layer=package.position.layer.find_layer_number(), bin_id=p_advisor.find_bin_containing_package(package).bin_id)
    else:
        return 'P: error - position not set'

def bluetooth_format_bin(bin):
    return 'B: {x} {y} {max_layers} {bin_id}\n'.format(x=bin.length*bluetooth_scale, y=bin.width*bluetooth_scale, max_layers=bin.max_layers, bin_id=bin.bin_id)
	
def setup_server():
    s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    s.bind((MAC_ADDR, PORT))
    s.listen(BACKLOG)
    bluetooth.advertise_service(s, "PiServer Service", SERVICE_ID)
    print('Server startet {mac_addr}...'.format(mac_addr=MAC_ADDR))
    return s

def handle_client(client):      ###Reuse this code to exception handle the BT communication
    try:
        client.send("B: 500 250 3 \n")
        client.send("P: 0 0 100 150 1 1 \n")
    except EOFError:
        client.close()
    except bluetooth.btcommon.BluetoothError:
        client.close()

def make_client(server_socket):
    client, client_info = server_socket.accept()
    print('Accepting connection from {clientInfo}'.format(clientInfo=client_info))
    return client
	
def blue_test(server_input):
	global BTReady, client, bin_buf_lock #Tell interpreter to use global BTReady instead of create local var
	print('Thread 2 started')
	client = make_client(server_input)
	bin_buf_lock.acquire()
	for b in bin_buf:
		client.send(bluetooth_format_bin(b))
	for p in pack_buf:
		client.send(bluetooth_format_package(p, pa))
	BTReady = True
	print('Thread 2 is done')
	bin_buf_lock.release()
	
################# BEGIN MAIN ##########

server = setup_server()
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
		print('Hillary is jail')
		if new_bin:
			client.send(bluetooth_format_bin(pa.find_bin_containing_package(package)))
		client.send(bluetooth_format_package(package, pa))
	else:
		print('Trump is great')
		if new_bin:
			bin_buf.append(pa.find_bin_containing_package(package))
		pack_buf.append(package)
	bin_buf_lock.release()
	time.sleep(2)