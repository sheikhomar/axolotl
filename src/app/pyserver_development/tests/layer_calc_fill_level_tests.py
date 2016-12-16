import unittest

from lib import Package
from lib import Layer

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