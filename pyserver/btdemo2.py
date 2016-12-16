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
    bin1 = Bin(width=5, length=10, max_layers=3)

    p1 = Package(width=3, length=2)
    p2 = Package(width=2, length=2)
    p3 = Package(width=4, length=2)
    p4 = Package(width=3, length=2)
    p5 = Package(width=4, length=2, colour = 3)
    p6 = Package(width=2, length=2)
    p7 = Package(width=2, length=2)
    p8 = Package(width=4, length=2)
    p9 = Package(width=4, length=2)
    p10 = Package(width=2, length=2, colour = 3)
    p11 = Package(width=4, length=2)
    p12 = Package(width=4, length=2)
    p13 = Package(width=2, length=2)
    p14 = Package(width=4, length=2)
    p15 = Package(width=3, length=2, colour = 2)
    p16 = Package(width=4, length=2, colour = 1)
    p17 = Package(width=2, length=2, colour = 2)

    pa = PackingAdvisor(bin1)
    client = server_loop(s)
    client.send(bluetooth_format_bin(bin1))
    client.send(bluetooth_format_bin(pa.bins_foreign[0]))
    pa.handle(p1)
    client.send(bluetooth_format_package(p1, pa))
    pa.handle(p2)
    client.send(bluetooth_format_package(p2, pa))
    pa.handle(p3)
    client.send(bluetooth_format_package(p3, pa))
    pa.handle(p4)
    client.send(bluetooth_format_package(p4, pa))
    pa.handle(p5)
    client.send(bluetooth_format_package(p5, pa))
    pa.handle(p6)
    time.sleep(2)
    client.send(bluetooth_format_package(p6, pa))
    pa.handle(p7)
    client.send(bluetooth_format_package(p7, pa))
    pa.handle(p8)
    client.send(bluetooth_format_package(p8, pa))
    pa.handle(p9)
    client.send(bluetooth_format_package(p9, pa))
    pa.handle(p10)
    time.sleep(3)
    client.send(bluetooth_format_package(p10, pa))
    pa.handle(p11)
    client.send(bluetooth_format_package(p11, pa))
    pa.handle(p12)
    client.send(bluetooth_format_package(p12, pa))
    pa.handle(p13)
    client.send(bluetooth_format_package(p13, pa))
    pa.handle(p14)
    client.send(bluetooth_format_package(p14, pa))
    pa.handle(p15)
    time.sleep(2)
    client.send(bluetooth_format_package(p15, pa))
    pa.handle(p16)
    client.send(bluetooth_format_package(p16, pa))
    pa.handle(p17)
    client.send(bluetooth_format_package(p17, pa))

