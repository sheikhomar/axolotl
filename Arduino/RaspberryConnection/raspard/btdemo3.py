from lib import *
import bluetooth
import logging
import time

BLib = BlueLib()



s = BLib.setup_server()

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
	client = BLib.make_client(s)
	client.send(BLib.bluetooth_format_bin(bin1))
	client.send(BLib.bluetooth_format_bin(pa.bins_foreign[0]))

	for p in pack_list:
		if pa.handle(p):
			client.send(BLib.bluetooth_format_bin(pa.find_bin_containing_package(p)))
		client.send(BLib.bluetooth_format_package(p, pa))
		time.sleep(1)

