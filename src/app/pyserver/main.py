from lib import *


bin1 = Bin(width=8, length=8, max_layers=3)
bin2 = Bin(width=8, length=8, max_layers=4)

p1 = Package(width=4, length=2)
p2 = Package(width=4, length=2)
p3 = Package(width=2, length=2)

pa = PackingAdvisor([bin1, bin2])
pa.handle(p1)
pa.handle(p2)
pa.handle(p3)


