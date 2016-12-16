import unittest

from lib import Package
from lib import Layer

class LayerFindOccupyingPackageTests(unittest.TestCase):
    
    def setUp(self):
        self.layer = Layer(10, 10)
        self.p1 = Package(width=5, length=5)
        self.p2 = Package(width=5, length=5)
        
    def test_should_return_none_if_layer_contains_no_packages(self):
        for x in range(-10, 21):
            for y in range(-10, 21):
                self.assertIsNone(self.layer.find_occupying_package(x, y))
        
    def test_should_return_valid_package_instance(self):
        self.layer.pack(self.p1, 0, 0)
        self.layer.pack(self.p2, 5, 5)
        
        for x in range(0, 5):
            for y in range(0, 5):
                self.assertIs(self.layer.find_occupying_package(x, y), self.p1)

        for x in range(5, 10):
            for y in range(5, 10):
                self.assertIs(self.layer.find_occupying_package(x, y), self.p2)

    def test_should_return_none_if_no_package_found(self):
        self.layer.pack(self.p1, 0, 0)
        self.layer.pack(self.p2, 5, 5)
        
        for x in range(5, 10):
            for y in range(0, 5):
                self.assertIsNone(self.layer.find_occupying_package(x, y))

        for x in range(0, 5):
            for y in range(5, 10):
                self.assertIsNone(self.layer.find_occupying_package(x, y))