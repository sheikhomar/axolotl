from lib import *


bin1 = Bin(width=5, length=10, max_layers=3)

bluetooth_scale = 1

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

def format_serial_package(input_array):
    if len(input_array) == 7:
        return Package(width=int.from_bytes(input_array[4], byteorder='little'), length=int.from_bytes(input_array[3], byteorder='little'), colour=int.from_bytes(input_array[6], byteorder='little'))
    else:
        raise InvalidArgError('Input to format_serial_package was not array of length 7')

p1 = Package(width=3, length=2)
#input = ['r', 4, 'p', 2, 2, 3, 2] for serial input debugging


pa = PackingAdvisor(bin1)

print((12).to_bytes(length=1, byteorder='little'))
print(int.from_bytes(b'\x01', byteorder = 'little')) 
print(b'\xf0'.hex())