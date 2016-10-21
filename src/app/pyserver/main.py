from lib import *


bin1 = Bin(width=5, length=10, max_layers=3)
bin2 = Bin(width=8, length=8, max_layers=4)

bluetooth_scale = 50

def bluetooth_format_package(package):
    if package.position is not None:
        return 'P: {x} {y} {length} {width} {colour} {layer}\n'.format(x=package.position.x*bluetooth_scale, y=package.position.y*bluetooth_scale, length=package.length*bluetooth_scale, width=package.width*bluetooth_scale, colour=package.colour, layer=package.position.layer.find_layer_number())
    else:
        return 'P: error - position not set'

def bluetooth_format_bin(bin):
    return 'B: {x} {y} {max_layers}\n'.format(x=bin.length*bluetooth_scale, y=bin.width*bluetooth_scale, max_layers=bin.max_layers)

p1 = Package(width=3, length=2)
p2 = Package(width=2, length=2)
p3 = Package(width=4, length=2)
p4 = Package(width=3, length=2)
p5 = Package(width=4, length=2)
p6 = Package(width=2, length=2)
p7 = Package(width=2, length=2)
p8 = Package(width=4, length=2)
p9 = Package(width=4, length=2)
p10 = Package(width=2, length=2)
p11 = Package(width=4, length=2)
p12 = Package(width=4, length=2)
p13 = Package(width=4, length=2)
p14 = Package(width=4, length=2)


pa = PackingAdvisor([bin1, bin2])
pa.handle(p1)
pa.handle(p2)
pa.handle(p3)
pa.handle(p4)
pa.handle(p5)
pa.handle(p6)
pa.handle(p7)
pa.handle(p8)
pa.handle(p9)
pa.handle(p10)
pa.handle(p11)
pa.handle(p12)
pa.handle(p13)
pa.handle(p14)


print(bluetooth_format_bin(bin1))
print(bluetooth_format_package(p1))
print(bluetooth_format_package(p2))
print(bluetooth_format_package(p3))
print(bluetooth_format_package(p4))
print(bluetooth_format_package(p5))
print(bluetooth_format_package(p6))
print(bluetooth_format_package(p7))
print(bluetooth_format_package(p8))
print(bluetooth_format_package(p9))
print(bluetooth_format_package(p10))
print(bluetooth_format_package(p11))
print(bluetooth_format_package(p12))
print(bluetooth_format_package(p13))
print(bluetooth_format_package(p14))
