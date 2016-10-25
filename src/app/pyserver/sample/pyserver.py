import bluetooth
import logging

log = logging.getLogger(__name__)

# The MAC address of a Bluetooth adapter on the server.
# The server might have multiple Bluetooth adapters.
MAC_ADDR = '00:0C:78:33:A5:63'
PORT = 0 #
BACKLOG = 3
BUFFER_SIZE = 1024
SERVICE_ID = "c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd"

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
    while True:
        client, client_info = server_socket.accept()
        print('Accepting connection from {clientInfo}'.format(clientInfo=client_info))
        handle_client(client) #Have it return 'client' to use in main. Then just do 'client.send'

s = setup_server()
server_loop(s)