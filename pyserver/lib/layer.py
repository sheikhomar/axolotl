from .position import *
from .invalid_arg_error import *

class Layer:
    def __init__(self, width, length, previous_layer = None):
        self.width = width
        self.length = length
        self.packages = []
        self.previous_layer = previous_layer

    # Calculate fill level and returns a number from 0 to 1
    def calc_fill_level(self):
        packages_area = 0
        for package in self.packages:
            packages_area = packages_area + (package.width * package.length)
        max_area = self.width * self.length
        return packages_area / max_area

    def pack(self, package, x, y):
        if x + package.length > self.length or y + package.width > self.width:
            raise InvalidArgError('Package area is too large to find within the bounds of this layer.')

        for p in self.packages:
            if p is package:
                raise InvalidArgError('Package is already packed.')
        
        if self.find_occupying_package(x, y) is not None:
            raise InvalidArgError('Package overlaps already packed package')

        package.position = PackedPosition(x, y, self)
        self.packages.append(package)

    def find_occupying_package(self, x, y):
        for p in self.packages:
            if p.position_is_taken(x, y):
                return p
        
        return None
    
    def find_layer_number(self):
        i = 1
        layer = self
        while layer.previous_layer is not None:
            layer = layer.previous_layer
            i = i + 1

        return i