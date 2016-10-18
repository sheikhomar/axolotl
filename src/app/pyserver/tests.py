import unittest

from lib import Bin
from lib import Package
from lib import PackedPosition
from lib import PackingAdvisor
from lib import Layer
from lib import InvalidArgError

class PackagePositionIsTakenTests(unittest.TestCase):

    def setUp(self):
        self.bin = Bin(width=100, length=100, max_layers=3)
        
    def test_should_return_false_if_no_position(self):
        package = Package(width=5, length=5)
        self.assertFalse(package.position_is_taken(0, 0))

    def test_should_return_true_if_within_area(self):
        package = Package(width=5, length=5)
        package.position = PackedPosition(0, 0)
        for x in range(0, 5): # Range is from 0 to 4 inclusive
            for y in range(0, 5):
                self.assertTrue(package.position_is_taken(x, y))

    def test_should_return_false_if_outside_area_positive(self):
        package = Package(width=5, length=5)
        package.position = PackedPosition(0, 0)
        # Range is from 5 to 50
        for x in range(5, 51):
            for y in range(5, 51):
                self.assertFalse(package.position_is_taken(x, y))

    def test_should_return_false_if_outside_area_negative(self):
        package = Package(width=5, length=5)
        package.position = PackedPosition(0, 0)
        # Range is from -50 to -1
        for x in range(-50, 0):
            for y in range(-50, 0):
                self.assertFalse(package.position_is_taken(x, y))
                
class PackingAdvisorHandleTests(unittest.TestCase):
    def test_handle(self):
        bin1 = Bin(width=8, length=8, max_layers=1)

        p1 = Package(width=4, length=2)
        p2 = Package(width=4, length=2)
        p3 = Package(width=2, length=2)

        pa = PackingAdvisor([bin1])
        pa.handle(p1)
        pa.handle(p2)
        pa.handle(p3)

        self.assertEqual(bin1.current_layer.packages[0].position.x, 0)
        self.assertEqual(bin1.current_layer.packages[0].position.y, 0)
        self.assertEqual(bin1.current_layer.packages[1].position.x, 0)
        self.assertEqual(bin1.current_layer.packages[1].position.y, 4)
        self.assertEqual(bin1.current_layer.packages[2].position.x, 2)
        self.assertEqual(bin1.current_layer.packages[2].position.y, 0)


class LayerFindOccupyingPackageTests(unittest.TestCase):
    def test_should_return_none_if_layer_contains_no_packages(self):
        layer = Layer(10, 10)
        for x in range(-10, 21):
            for y in range(-10, 21):
                self.assertIsNone(layer.find_occupying_package(x, y))
        
    def test_should_return_valid_package_instance(self):
        layer = Layer(10, 10)
        p1 = Package(width=5, length=5)
        p2 = Package(width=5, length=5)
        layer.pack(p1, 0, 0)
        layer.pack(p2, 5, 5)
        
        for x in range(0, 5):
            for y in range(0, 5):
                self.assertIs(layer.find_occupying_package(x, y), p1)

        for x in range(5, 10):
            for y in range(5, 10):
                self.assertIs(layer.find_occupying_package(x, y), p2)

    def test_should_return_none_if_no_package_found(self):
        layer = Layer(10, 10)
        p1 = Package(width=5, length=5)
        p2 = Package(width=5, length=5)
        layer.pack(p1, 0, 0)
        layer.pack(p2, 5, 5)
        
        for x in range(5, 10):
            for y in range(0, 5):
                self.assertIsNone(layer.find_occupying_package(x, y))

        for x in range(0, 5):
            for y in range(5, 10):
                self.assertIsNone(layer.find_occupying_package(x, y))

                
class LayerCalcFillLevelTests(unittest.TestCase):
    def test_should_return_zero_if_layer_is_empty(self):
        layer = Layer(10, 10)
        self.assertEqual(layer.calc_fill_level(), 0)
        
    def test_should_return_correct_fill_ratio_1(self):
        layer = Layer(10, 10)
        p1 = Package(width=5, length=5)
        p2 = Package(width=5, length=5)

        layer.pack(p1, 0, 0)
        self.assertEqual(layer.calc_fill_level(), 0.25)
        layer.pack(p2, 5, 5)
        self.assertEqual(layer.calc_fill_level(), 0.5)

class LayerPackTests(unittest.TestCase):
    def test_should_add_packages_to_list(self):
        layer = Layer(10, 10)
        p1 = Package(width=5, length=5)
        p2 = Package(width=5, length=5)

        self.assertEqual(len(layer.packages), 0)
        layer.pack(p1, 0, 0)
        self.assertEqual(len(layer.packages), 1)
        layer.pack(p2, 5, 5)

        self.assertEqual(len(layer.packages), 2)
        self.assertIs(layer.packages[0], p1)
        self.assertIs(layer.packages[1], p2)
        
    def test_should_not_allow_same_package_to_be_repacked(self):
        layer = Layer(10, 10)
        p1 = Package(width=5, length=5)
        p2 = Package(width=5, length=5)
        layer.pack(p1, 0, 0)

        with self.assertRaises(InvalidArgError):
            layer.pack(p1, 5, 5)
        
    def test_should_not_pack_outside_defined_area(self):
        p1 = Package(width=5, length=5)
        
        with self.assertRaises(InvalidArgError):
            for x in range(6, 16):
                for y in range(0, 5):
                    layer = Layer(10, 10)
                    layer.pack(p1, x, y)

        with self.assertRaises(InvalidArgError):
            for x in range(0, 5):
                for y in range(6, 16):
                    layer = Layer(10, 10)
                    layer.pack(p1, x, y)
        


if __name__ == '__main__':
    unittest.main()