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

    def position_is_taken(self, x, y):
        #Assumes position is initialized and test proposed position of new package
        if self.position != None:
            return (self.position.x <= x and x < self.position.x + self.length) and (self.position.y <= y and y < self.position.y + self.width)
        else:
            return False

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
        self.x = 0
        self.y = 0
        self.package_fits = False

    def handle(self, package):
        print('Handling package...')
        
        bin = self.find_bin(package) #ATM just returns bin[0]
        
        self.find_x_y(bin.current_layer, package)

        if self.package_fits:
            print('Package fits ')
            bin.current_layer.pack(package, self.x, self.y)
        else:
            print('Package does not fit in current layer')
        
    
    def find_bin(self, package):
        print('Find the correct bin for package {0}'.format(package))
        return self.bins[0]

    def find_x_y(self, layer, package_to_pack):
        self.x = 0
        self.y = 0 
        self.propose_x_y(layer, package_to_pack) #Recursively propose x,y coordinates until a valid set is found

    def propose_x_y(self, layer, package_to_pack):
        for p in layer.packages:
            if p.position_is_taken(self.x, self.y):
                if self.y + p.width + package_to_pack.width >= layer.width: #package_to_pack does not fit in current column
                    self.y = 0 
                    if self.x + layer.find_occupying_package(self.x, self.y).length + package_to_pack.length >= layer.length:
                        self.package_fits = False #No further columns are available in layer
                    else:
                        self.x = self.x + layer.find_occupying_package(self.x, self.y).length #start new column
                        self.propose_x_y
                        return
                else: #package_to_pack does fit further down in current column
                    self.y = self.y + p.width
                    self.propose_x_y(layer, package_to_pack)
                    return
        
        self.package_fits = True
        return #This is the case when the position is not taken by any other package. No position that would go out of bounds is ever proposed
                            



bin1 = Bin(width=8, length=8, max_layers=3)
bin2 = Bin(width=8, length=8, max_layers=4)

p1 = Package(width=4, length=2)
p2 = Package(width=4, length=2)
p3 = Package(width=2, length=2)

pa = PackingAdvisor([bin1, bin2])
pa.handle(p1)
pa.handle(p2)
pa.handle(p3)





