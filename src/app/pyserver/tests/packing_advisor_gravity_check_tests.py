import unittest

from lib import Bin
from lib import Package
from lib import PackingAdvisor

class PackingAdvisorGravityCheckTest(unittest.TestCase):
    
    def test_gravity_check(self):
        bin1 = Bin(4,4,3)
        pa = PackingAdvisor(bin1)
        p1 = Package(2, 2)
        p2 = Package(4, 2)
        p3 = Package(4, 2)
        p4 = Package(4, 2)

        pa.handle(p1)
        pa.handle(p2)

        pa.bins[0].new_layer()
        pa.x = 0
        pa.y = 0
        self.assertFalse(pa.calc_gravity(pa.bins[0].current_layer, p3) > 0.75)
        pa.handle(p3)

        pa.bins[0].new_layer()
        pa.x = 2
        pa.y = 0
        self.assertTrue(pa.calc_gravity(pa.bins[0].current_layer, p4) > 0.75)
        


