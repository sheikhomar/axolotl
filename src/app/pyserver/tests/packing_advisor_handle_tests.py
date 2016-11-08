import unittest

from lib import Bin
from lib import Package
from lib import PackingAdvisor
from lib import InvalidArgError

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

    def test_handle_advanced(self):
        #The code in this case is the main.py() at the time of every feature working first time (31/10 2016)
        bin1 = Bin(width=5, length=10, max_layers=3)

        p1 = Package(width=3, length=2)
        p2 = Package(width=2, length=2)
        p3 = Package(width=4, length=2)
        p4 = Package(width=3, length=2)
        p5 = Package(width=4, length=2, colour = 3)
        p6 = Package(width=2, length=2)
        p7 = Package(width=2, length=2)
        p8 = Package(width=4, length=2)
        p9 = Package(width=4, length=2)
        p10 = Package(width=2, length=2, colour = 3)
        p11 = Package(width=4, length=2)
        p12 = Package(width=4, length=2)
        p13 = Package(width=2, length=2)
        p14 = Package(width=4, length=2)
        p15 = Package(width=3, length=2, colour = 2)
        p16 = Package(width=4, length=2, colour = 2)
        p17 = Package(width=2, length=2, colour = 2)
        
        pa = PackingAdvisor(bin1)

        pa.handle(p1)
        pa.handle(p2)
        pa.handle(p3)
        pa.handle(p4)
        pa.handle(p5)
        pa.handle(p6)
        pa.handle(p7)
        pa.handle(p8)
        pa.handle(p9)
        pa.handle(p10)
        pa.handle(p11)
        pa.handle(p12)
        pa.handle(p13)
        pa.handle(p14)
        pa.handle(p15)
        pa.handle(p16)
        pa.handle(p17)

        self.assertEqual(p1.position.x, 0)    
        self.assertEqual(p1.position.y, 0)
        self.assertEqual(p1.position.layer.find_layer_number(), 1)
        self.assertEqual(p2.position.x, 0)    
        self.assertEqual(p2.position.y, 3)
        self.assertEqual(p2.position.layer.find_layer_number(), 1)
        self.assertEqual(p3.position.x, 2)    
        self.assertEqual(p3.position.y, 0)
        self.assertEqual(p3.position.layer.find_layer_number(), 1)
        self.assertEqual(p4.position.x, 4)    
        self.assertEqual(p4.position.y, 0)
        self.assertEqual(p4.position.layer.find_layer_number(), 1)
        self.assertEqual(p5.position.x, 6)    
        self.assertEqual(p5.position.y, 0)
        self.assertEqual(p5.position.layer.find_layer_number(), 1)
        self.assertEqual(p6.position.x, 4)    
        self.assertEqual(p6.position.y, 3)
        self.assertEqual(p6.position.layer.find_layer_number(), 1)
        self.assertEqual(p7.position.x, 8)    
        self.assertEqual(p7.position.y, 0)
        self.assertEqual(p7.position.layer.find_layer_number(), 1)
        self.assertEqual(p8.position.x, 0)    
        self.assertEqual(p8.position.y, 0)
        self.assertEqual(p8.position.layer.find_layer_number(), 2)
        self.assertEqual(p9.position.x, 2)
        self.assertEqual(p9.position.y, 0)
        self.assertEqual(p9.position.layer.find_layer_number(), 2)
        self.assertEqual(p10.position.x, 8)    
        self.assertEqual(p10.position.y, 2)
        self.assertEqual(p10.position.layer.find_layer_number(), 1)
        self.assertEqual(p11.position.x, 4)    
        self.assertEqual(p11.position.y, 0)
        self.assertEqual(p11.position.layer.find_layer_number(), 2)
        self.assertEqual(p12.position.x, 8)    
        self.assertEqual(p12.position.y, 0)
        self.assertEqual(p12.position.layer.find_layer_number(), 2)
        self.assertEqual(p13.position.x, 0)    
        self.assertEqual(p13.position.y, 0)
        self.assertEqual(p13.position.layer.find_layer_number(), 3)
        self.assertEqual(p14.position.x, 2)    
        self.assertEqual(p14.position.y, 0)
        self.assertEqual(p14.position.layer.find_layer_number(), 3)
        #Last three packages are in foreign destination bin
        self.assertEqual(p15.position.x, 0)    
        self.assertEqual(p15.position.y, 0)
        self.assertEqual(p15.position.layer.find_layer_number(), 1)
        self.assertEqual(p16.position.x, 2)    
        self.assertEqual(p16.position.y, 0)
        self.assertEqual(p16.position.layer.find_layer_number(), 1)
        self.assertEqual(p17.position.x, 0)    
        self.assertEqual(p17.position.y, 3)
        self.assertEqual(p17.position.layer.find_layer_number(), 1)

        #Lastly the bin_id's are checked
        self.assertEqual(pa.find_bin_containing_package(p8).bin_id, 1)
        self.assertEqual(pa.find_bin_containing_package(p15).bin_id, 2)

    def test_handle_package_larger_than_layer(self):
        bin1 = Bin(width=5, length=10, max_layers=1)

        p1 = Package(width=6, length=2)
        p2 = Package(width=2, length=11)

        pa = PackingAdvisor(bin1)

        with self.assertRaises(InvalidArgError):
            pa.handle(p1)
        with self.assertRaises(InvalidArgError):
            pa.handle(p2)