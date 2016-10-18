import unittest

from lib import Package
from lib import PackedPosition

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