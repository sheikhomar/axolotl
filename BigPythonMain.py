import serial
import time
import logging
import bluetooth

from lib import *


####### Serial communication instance    #########
ser = serial.Serial('/dev/ttyUSB0',57600)

def format_serial_package(input_array):
    if len(input_array) == 7:
        return Package(int.from_bytes(input_array[4], byteorder='little'), int.from_bytes(input_array[3], byteorder='little'), int.from_bytes(input_array[6], byteorder='little'))
    else:
        raise InvalidArgError('Input to format_serial_package was not array of length 7')

def read_x_bytes(ser, data, x):
	i = 0
	while True:
		if ser.inWaiting() >= 1:
			data.append(ser.read())
			i = i + 1
			if i == x:
				return data

def serial_read(ser, data):	
	#Simulated Do-while loop that reads over initial noise
	while True:
		if ser.inWaiting() >= 3:
			data.append(ser.read())
			if data[0] == b'r' or data[0] == b'n' or data[0] == b'a':
				print('YES')
				break
			else:
				print('waiting: ' + str(ser.inWaiting()) + '      just read: ' + str(data[0]))
				data.clear()
		else:
			print('Not a full message in waiting')
			return (data, False)
	
	
	#If message is not for raspberry PI, skip over it		
	if data[0] == b'a' or data[0] == b'n':
		while True:
			if ser.inWaiting() >= 1:
				data.append(int.from_bytes(ser.read(), byteorder='little'))
				if data[1] <= 11:
					data = read_x_bytes(ser, data, x = data[1] + 1)
					data.clear()
					return serial_read(ser, data)
				else:
					print('Implied message for Non-Raspberry, but length was incompatible: {}'.format(data[1]))
					data.clear()
					return serial_read(ser, data)
	
	if data[0] == b'r':
		while True:
			if ser.inWaiting() >= 1:
				data.append(int.from_bytes(ser.read(), byteorder='little'))
				if data[1] <= 11:
					data = read_x_bytes(ser, data, x=data[1] + 1)
					return (data, True)
				else:
					print('Implied message for Raspberry, but length was incompatible: {}'.format(data[1]))
					data.clear()
					return serial_read(ser, data)
	else:
		raise ArithmeticError('First byte does not indicate proper message')
		
def serial_write_push(ser, bin_id):
	ser.write(b'a')
	ser.write((1).to_bytes(1, byteorder='little'))
	ser.write(b'p')
	ser.write(((bin_id % 2) + 1).to_bytes(1, byteorder='little'))

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

	
	
################################################                  #####  #####    #    ###    #####
#											   #                  #        #     # #   #  #     #
#               BIG PYTHON MAIN                #                  #####    #    #####  ###      #
#											   #	                  #    #    #   #  #  #     #
################################################                  #####    #    #   #  #   #    #
s = setup_server()
client = make_client(s)	
	

	

bin1 = Bin(width=5, length=10, max_layers=3)
pa = PackingAdvisor(bin1)

client.send(bluetooth_format_bin(pa.bins[0]))
client.send(bluetooth_format_bin(pa.bins_foreign[0]))

data = []

while True:
	input_serial = serial_read(ser, data)
	if input_serial[1]:
		print('Package received')
		p1 = format_serial_package(input_serial[0])
		if pa.handle(p1):
			client.send(bluetooth_format_bin(pa.find_bin_containing_package(p1)))
		print(((pa.find_bin_containing_package(p1).bin_id % 2) + 1).to_bytes(1, byteorder='little'))
		serial_write_push(ser, pa.find_bin_containing_package(p1).bin_id)
		client.send(bluetooth_format_package(package))
	data.clear()
	time.sleep(1)
