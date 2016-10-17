from .position import *

class Layer:
    def __init__(self, width, length):
        self.width = width
        self.length = length
        self.packages = []

    # Calculate fill level and returns a number from 0 to 1
    def fill_level(self):
        packages_area = 0
        for package in self.packages:
            packages_area = packages_area + (package.width * package.length)
        max_area = self.width * self.length
        return packages_area / max_area

    def pack(self, package, x, y):
        package.position = PackedPosition(x, y)
        self.packages.append(package)
