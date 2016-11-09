import time
import logging
import bluetooth
import _thread

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
    if package.position is not None:
        return 'P: {x} {y} {length} {width} {colour} {layer} {bin_id}\n'.format(x=package.position.x*bluetooth_scale, y=package.position.y*bluetooth_scale, length=package.length*bluetooth_scale, width=package.width*bluetooth_scale, colour=package.colour, layer=package.position.layer.find_layer_number(), bin_id=p_advisor.find_bin_containing_package(package).bin_id)
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
	global BTReady, client #Tell interpreter to use global BTReady instead of create local var
	print('Thread 2 started')
	client = make_client(server_input)
	BTReady = True
	print('Thread 2 is done')
	
################# BEGIN MAIN ##########

server = setup_server()
client = None
BTReady = False

_thread.start_new_thread(blue_test, (server,))

while(True):
	if BTReady:
		print('Hillary is jail')
	else:
		print('Trump is great')
	time.sleep(2)