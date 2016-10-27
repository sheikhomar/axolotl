import serial
import time
import logging
import bluetooth

from lib import *


####### Serial communication instance    #########
ser = serial.Serial('/dev/ttyACM0',9600)


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
        return 'P: {x} {y} {length} {width} {colour} {layer} {bin_id}\n'.format(x=package.position.x*bluetooth_scale, y=package.position.y*bluetooth_scale, length=package.length*bluetooth_scale, width=package.width*bluetooth_scale, colour=package.colour, layer=package.position.layer.find_layer_number(), bin_id=p_advisor.find_bin_id_containing_package(package))
    else:
        return 'P: error - position not set'

def bluetooth_format_bin(bin):
    return 'B: {x} {y} {max_layers}\n'.format(x=bin.length*bluetooth_scale, y=bin.width*bluetooth_scale, max_layers=bin.max_layers)
	
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
    return client #Have it return 'client' to use in main. Then just do 'client.send'

s = setup_server()
client = make_client(s)	
	

	

bin1 = Bin(width=5, length=10, max_layers=3)
pa = PackingAdvisor(bin1)

client.send(bluetooth_format_bin(pa.bins[0])
client.send(bluetooth_format_bin(pa.bins_foreign[0])

while True:
	input_string = ser.readline()
	package = format_serial_package(input_string) #####Make this function#####
	if pa.handle(package):
		client.send(bluetooth_format_bin(pa.find_bin_containing_package(package)))
	ser.sendline(str(pa.find_bin_containing_package(package).bin_id % 2))
	client.send(bluetooth_format_package(package))