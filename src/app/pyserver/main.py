from lib import *


bin1 = Bin(width=5, length=10, max_layers=1)
bin2 = Bin(width=8, length=8, max_layers=4)

p1 = Package(width=3, length=2)
p2 = Package(width=2, length=2)
p3 = Package(width=4, length=2)
p4 = Package(width=3, length=2)
p5 = Package(width=4, length=2)
p6 = Package(width=2, length=2)
p7 = Package(width=4, length=2)

pa = PackingAdvisor([bin1, bin2])
pa.handle(p1)
pa.handle(p2)
pa.handle(p3)
pa.handle(p4)
pa.handle(p5)
pa.handle(p6)
pa.handle(p7)


