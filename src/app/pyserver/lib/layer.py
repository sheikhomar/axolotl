from .position import *
from .invalid_arg_error import *

class Layer:
    def __init__(self, width, length):
        self.width = width
        self.length = length
        self.packages = []

    # Calculate fill level and returns a number from 0 to 1
    def calc_fill_level(self):
        packages_area = 0
        for package in self.packages:
            packages_area = packages_area + (package.width * package.length)
        max_area = self.width * self.length
        return packages_area / max_area

    def pack(self, package, x, y):
        for p in self.packages:
            if p is package:
                raise InvalidArgError('Package is already packed.')

        package.position = PackedPosition(x, y)
        self.packages.append(package)

    def find_occupying_package(self, x, y):
        for p in self.packages:
            if p.position_is_taken(x, y):
                return p
        
        return None
