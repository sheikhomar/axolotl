import unittest

from lib import Package
from lib import PackingAdvisor
from lib import Bin

class PackingAdvisorAirspaceTests(unittest.TestCase):
    
    def test_airspace_true(self): #Returns true when airspace is empty
        
        #Arrange
        bin1 = Bin(4,4,2)
        pa = PackingAdvisor(bin1)
        p1 = Package(4,2)
        p2 = Package(2,2)
        p3 = Package(4,2)
        p4 = Package(2,2)

        #Act
        pa.handle(p1)
        pa.handle(p2)
        pa.handle(p3)

        #Assert
        pa.x = 2 #Is set manually to de-couple the test of airspace from find_x_y_lower_layer
        pa.y = 2
        self.assertTrue(pa.check_airspace(bin1, p4))
        

    def test_airspace_false(self): #Returns false when airspace is not empty
                
        #Arrange
        bin1 = Bin(4,4,2)
        pa = PackingAdvisor(bin1)
        p1 = Package(3,2)
        p2 = Package(4,2)
        p3 = Package(4,2)
        p4 = Package(1,2)

        #Act
        pa.handle(p1)
        pa.handle(p2)
        pa.handle(p3)

        #Assert
        pa.x = 0
        pa.y = 3
        self.assertFalse(pa.check_airspace(bin1, p4))


