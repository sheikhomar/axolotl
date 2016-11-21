import bluetooth

class BlueLib:
	def __init__(self):
		# The MAC address of a Bluetooth adapter on the server.
		# The server might have multiple Bluetooth adapters.
		self.MAC_ADDR = '00:0C:78:33:A5:63'
		self.PORT = 0 #
		self.BACKLOG = 3
		self.BUFFER_SIZE = 1024
		self.SERVICE_ID = "c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd"
	
	def bluetooth_format_package(self, package, p_advisor):
		fragile = 0
	
		if package.is_fragile:
			fragile = 1
		if package.position is not None:
			return 'P: {x} {y} {length} {width} {height} {colour} {fragile} {layer} {bin_id}\n'.format(x=package.position.x, y=package.position.y, length=package.length, width=package.width, height=package.height, colour=package.colour, fragile=fragile, layer=package.position.layer.find_layer_number(), bin_id=p_advisor.find_bin_containing_package(package).bin_id)
		else:
			return 'P: error - position not set'
	
	def bluetooth_format_bin(self, bin):
		if bin.foreign == True:
			destination = 'France'
		else:
			destination = 'Isle,of,Mors'
		return 'B: {x} {y} {max_layers} {bin_id} {destination}\n'.format(x=bin.length, y=bin.width, max_layers=bin.max_layers, bin_id=bin.bin_id, destination=destination)
		
	def setup_server(self):
		s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
		s.bind((self.MAC_ADDR, self.PORT))
		s.listen(self.BACKLOG)
		bluetooth.advertise_service(s, "PiServer Service", self.SERVICE_ID)
		print('Server startet {mac_addr}...'.format(mac_addr=self.MAC_ADDR))
		return s
	
	def handle_client(self, client):      ###Reuse this code to exception handle the BT communication: Currently not used in BigPythonMain
		try:
			client.send("B: 500 250 3 \n")
			client.send("P: 0 0 100 150 1 1 \n")
		except EOFError:
			client.close()
		except bluetooth.btcommon.BluetoothError:
			client.close()
	
	def make_client(self, server_socket):
		client, client_info = server_socket.accept()
		print('Accepting connection from {clientInfo}'.format(clientInfo=client_info))
		return client 
