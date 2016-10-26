import unittest

from lib import Bin
from lib import Package
from lib import PackingAdvisor

class PackingAdvisorHandleTests(unittest.TestCase):
    def test_handle(self):
        bin1 = Bin(width=8, length=8, max_layers=1)

        p1 = Package(width=4, length=2)
        p2 = Package(width=4, length=2)
        p3 = Package(width=2, length=2)

        pa = PackingAdvisor(bin1)
        pa.handle(p1)
        pa.handle(p2)
        pa.handle(p3)

        self.assertEqual(bin1.current_layer.packages[0].position.x, 0)
        self.assertEqual(bin1.current_layer.packages[0].position.y, 0)
        self.assertEqual(bin1.current_layer.packages[1].position.x, 0)
        self.assertEqual(bin1.current_layer.packages[1].position.y, 4)
        self.assertEqual(bin1.current_layer.packages[2].position.x, 2)
        self.assertEqual(bin1.current_layer.packages[2].position.y, 0)