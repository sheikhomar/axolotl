import unittest

from lib import Bin

class BinNewLayerTests(unittest.TestCase):

    def test_new_layer_should_change_current_layer_and_append(self): #Always remember to name your function with a leading 'test_'
        bin1 = Bin(4,4,2)
        old_current_layer = bin1.current_layer
        self.assertEqual(len(bin1.layers), 1)
        bin1.new_layer()
        self.assertEqual(len(bin1.layers), 2)
        self.assertNotEqual(old_current_layer, bin1.current_layer)
        

    def test_new_layer_should_fail_if_no_more(self):
        bin2 = Bin(4,4,1)
        self.assertFalse(bin2.new_layer())

