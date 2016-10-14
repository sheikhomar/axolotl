class InvalidArgError(BaseException):
    def __init__(self, message):
        self.message = message

    def __str__(self):
        return self.message

class Position:
    pass

class PackedPosition(Position):
    def __init__(self, x, y):
        self.x = x
        self.y = y

class ConveyorPosition(Position):
    def __init(self, time):
        self.time = time


class Package:
    def __init__(self, width, length):
        self.width = width
        self.length = length
        #self.height = height
        #self.is_fragile = is_fragile
        #self.destination = destination
        self.position = None

    def __str__(self):
        return 'P[{0}x{1}]'.format(self.width, self.length)

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


class Bin:
    def __init__(self, width, length, max_layers):
        self.width = width
        self.length = length
        self.max_layers = max_layers
        self.current_layer = Layer(width=width, length=length)
        self.layers = [self.current_layer]

    # Closes the current layer and opens new layer in the bin.
    def new_layer(self):
        if len(self.layers) >= self.max_layers:
            err_msg = 'Max {0} layers are allowed for this bin.'.format(self.max_layers)
            raise InvalidArgError(err_msg)
        self.current_layer = Layer(width=self.width, length=self.length)
        self.layers.append(self.current_layer)
        
class PackingAdvisor:
    def __init__(self, bins = []):
        self.bins = bins

    def handle(self, package):
        print('Handling package...')
        
        bin = find_bin(package)
        if bin.current_layer.can_fit(package):
            print('Package fits ')
            x, y = CalcXY(package)
            bin.current_layer.pack(package, x, y)
        
    
    def find_bin(self, package):
        print('Find the correct bin for package {0}'.format(package))
        return bins[0]



bin1 = Bin(width=100, length=100, max_layers=3)
bin2 = Bin(width=100, length=100, max_layers=4)

p1 = Package(width=5, length=5)
p2 = Package(width=5, length=5)

pa = PackingAdvisor([bin1, bin2])
pa.handle(p1)
pa.handle(p2)



l1 = Layer(width=100, length=100)
l1.pack(p1, 0, 0)
l1.pack(p2, 0, 0)
print('Fill level {0}'.format(l1.fill_level()))

