import unittest

from lib import Package
from lib import Layer
from lib import InvalidArgError

class LayerPackTests(unittest.TestCase):
    
    def setUp(self):
        self.layer = Layer(10, 10)
        self.p1 = Package(width=5, length=5)
        self.p2 = Package(width=5, length=5)

    def test_should_add_packages_to_list(self):
        self.assertEqual(len(self.layer.packages), 0)
        self.layer.pack(self.p1, 0, 0)

        self.assertEqual(len(self.layer.packages), 1)
        self.layer.pack(self.p2, 5, 5)

        self.assertEqual(len(self.layer.packages), 2)
        self.assertIs(self.layer.packages[0], self.p1)
        self.assertIs(self.layer.packages[1], self.p2)
        
    def test_should_not_allow_same_package_to_be_repacked(self):
        self.layer.pack(self.p1, 0, 0)

        with self.assertRaises(InvalidArgError):
            self.layer.pack(self.p1, 5, 5)
        
    def test_should_not_pack_outside_defined_area(self):
        with self.assertRaises(InvalidArgError):
            for x in range(6, 16):
                for y in range(0, 5):
                    layer = Layer(10, 10)
                    layer.pack(self.p1, x, y)

        with self.assertRaises(InvalidArgError):
            for x in range(0, 5):
                for y in range(6, 16):
                    layer = Layer(10, 10)
                    layer.pack(self.p1, x, y)