import unittest

from lib import Bin
from lib import Package
from lib import PackedPosition
from lib import PackingAdvisor

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
    def test_handle_test(self):
        bin1 = Bin(width=8, length=8, max_layers=3)

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


if __name__ == '__main__':
    unittest.main()