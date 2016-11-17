from lib import *
import bluetooth
import logging
import time

log = logging.getLogger(__name__)

# The MAC address of a Bluetooth adapter on the server.
# The server might have multiple Bluetooth adapters.
MAC_ADDR = '00:0C:78:33:A5:63'
PORT = 0 #
BACKLOG = 3
BUFFER_SIZE = 1024
SERVICE_ID = "c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd"

bluetooth_scale = 50

def setup_server():
    s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    s.bind((MAC_ADDR, PORT))
    s.listen(BACKLOG)
    bluetooth.advertise_service(s, "PiServer Service", SERVICE_ID)
    print('Server startet {mac_addr}...'.format(mac_addr=MAC_ADDR))
    return s

def handle_client(client):
    try:
        client.send("B: 500 250 3 \n")
        client.send("P: 0 0 100 150 1 1 \n")
    except EOFError:
        client.close()
    except bluetooth.btcommon.BluetoothError:
        client.close()

def server_loop(server_socket):
    client, client_info = server_socket.accept()
    print('Accepting connection from {clientInfo}'.format(clientInfo=client_info))
    return client



def bluetooth_format_package(package, p_advisor):
    fragile = 0

    if package.is_fragile:
        fragile = 1
    if package.position is not None:
        return 'P: {x} {y} {length} {width} {height} {colour} {fragile} {layer} {bin_id}\n'.format(x=package.position.x*bluetooth_scale, y=package.position.y*bluetooth_scale, length=package.length*bluetooth_scale, width=package.width*bluetooth_scale, height=package.height*bluetooth_scale, colour=package.colour, fragile=fragile, layer=package.position.layer.find_layer_number(), bin_id=p_advisor.find_bin_containing_package(package).bin_id)
    else:
        return 'P: error - position not set'

def bluetooth_format_bin(bin):
    return 'B: {x} {y} {max_layers}\n'.format(x=bin.length*bluetooth_scale, y=bin.width*bluetooth_scale, max_layers=bin.max_layers)



s = setup_server()

while True:
	bin1 = Bin(width=8, length=8, max_layers=2)
	pack_list = []
	pack_list.append(Package(width=4, length=2, colour=0))
	pack_list.append(Package(width=2, length=2, colour=3))
	pack_list.append(Package(width=2, length=2, colour=0))
	pack_list.append(Package(width=4, length=2, colour=0))
	pack_list.append(Package(width=4, length=2, colour=3))
	pack_list.append(Package(width=8, length=2, colour=3))
	
	pack_list.append(Package(width=4, length=2, colour=1))
	pack_list.append(Package(width=4, length=2, colour=2))
	pack_list.append(Package(width=4, length=2, colour=2))
	
	pack_list.append(Package(width=4, length=2, colour=0))
	pack_list.append(Package(width=8, length=2, colour=3))
	pack_list.append(Package(width=4, length=2, colour=0))
	pack_list.append(Package(width=4, length=2, colour=0))
	pack_list.append(Package(width=4, length=2, colour=3))
	pack_list.append(Package(width=4, length=2, colour=0))
	pack_list.append(Package(width=8, length=2, colour=0))
	
	pack_list.append(Package(width=4, length=2, colour=1))
	
	
	pa = PackingAdvisor(bin1)
	client = server_loop(s)
	client.send(bluetooth_format_bin(bin1))
	client.send(bluetooth_format_bin(pa.bins_foreign[0]))

	for p in pack_list:
		if pa.handle(p):
			client.send(bluetooth_format_bin(pa.find_bin_containing_package(p)))
		client.send(bluetooth_format_package(p, pa))
		time.sleep(1)

