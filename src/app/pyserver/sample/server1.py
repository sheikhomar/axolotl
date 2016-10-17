import bluetooth
import logging
logging.basicConfig(
    filename="log.txt",
    format= "%(levelname)-10s %(asctime)s %(message)s",
    level=logging.DEBUG
)
logger = logging.getLogger(__name__)

# The MAC address of a Bluetooth adapter on the server. 
# The server might have multiple Bluetooth adapters.
MAC_ADDR = '00:0C:78:33:A5:63'
PORT_ANY = 0 # Use any port
BACKLOG = 3
BUFFER_SIZE = 1024
SERVICE_ID = "c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd"


def log(msg):
   print(msg)
   logger.debug(msg)

def create_server_socket():
    s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    s.bind((MAC_ADDR, PORT_ANY))
    s.listen(BACKLOG)
    bluetooth.advertise_service(s, "PiServer Service", SERVICE_ID)
    log( "Server started at {mac_addr}".format(mac_addr=MAC_ADDR) )
    return s

def handle_client(client, client_addr):
    try:
        client.send("Hello from piserver\n")
        while True:
            data = client.recv(BUFFER_SIZE)
            log( "Received data from client: {msg}".format(msg=data) )
            if data:
                client.send(data)
                client.send("\n")
    except EOFError:
        log("Client {} disconnected.".format(client_addr))
    except bluetooth.btcommon.BluetoothError as e:
        log("Client {} failed with error: {}".format(client_addr, e))
    finally:
        client.close()

def accept_connections(server_socket):
    while True:
        client, client_info = server_socket.accept()
        log( "Accepted connection from {}".format(client_info) )
        handle_client(client, client_info)

socket = create_server_socket()
accept_connections(socket)
