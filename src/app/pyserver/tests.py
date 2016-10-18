import unittest

from lib import Bin
from lib import Package
from lib import PackedPosition
from lib import PackingAdvisor
from lib import Layer
from lib import InvalidArgError


class PackagePositionIsTakenTests(unittest.TestCase):

    def setUp(self):
        self.package = Package(width=5, length=5)
        
    def test_should_return_false_if_no_position(self):
        self.assertFalse(self.package.position_is_taken(0, 0))

    def test_should_return_true_if_within_area(self):
        self.package.position = PackedPosition(0, 0)
        # Range is from 0 to 4 inclusive
        for x in range(0, 5): 
            for y in range(0, 5):
                self.assertTrue(self.package.position_is_taken(x, y))

    def test_should_return_false_if_outside_area_positive(self):
        self.package.position = PackedPosition(0, 0)
        # Range is from 5 to 50
        for x in range(5, 51):
            for y in range(5, 51):
                self.assertFalse(self.package.position_is_taken(x, y))

    def test_should_return_false_if_outside_area_negative(self):
        self.package.position = PackedPosition(0, 0)
        # Range is from -50 to -1
        for x in range(-50, 0):
            for y in range(-50, 0):
                self.assertFalse(self.package.position_is_taken(x, y))


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


class LayerCalcFillLevelTests(unittest.TestCase):
    
    def setUp(self):
        self.layer = Layer(10, 10)
        self.p1 = Package(width=5, length=5)
        self.p2 = Package(width=5, length=5)

    def test_should_return_zero_if_layer_is_empty(self):
        self.assertEqual(self.layer.calc_fill_level(), 0)
        
    def test_should_return_correct_fill_ratio_1(self):
        self.layer.pack(self.p1, 0, 0)
        self.assertEqual(self.layer.calc_fill_level(), 0.25)
        self.layer.pack(self.p2, 5, 5)
        self.assertEqual(self.layer.calc_fill_level(), 0.5)


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


if __name__ == '__main__':
    unittest.main()