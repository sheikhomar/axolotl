from lib import Bin
from lib import InvalidArgError

class PackingAdvisor:
    def __init__(self, bin):
        bin.bin_id = 1
        self.bins = [bin]
        bin2 = Bin(bin.width, bin.length, bin.max_layers)
        bin2.bin_id = 2
        self.bins_foreign = [bin2]

        # Internal variables
        self.x = 0
        self.y = 0
        self.package_fits = False
        self.current_bin = 0
        self.current_bin_foreign = 0
        self.next_bin_id = 3

        # Package metadata
        self.fragile = False #Currently obsolete
        self.foreign = False

    def handle(self, package):

        self.set_meta_data(package)
        
        bin = self.find_bin(package)
        
        if bin.current_layer.previous_layer is not None: #Perform test to see if package could be placed in layer below current
            if bin.current_layer.previous_layer.calc_fill_level() + ((package.length * package.width) / (bin.width * bin.length)) <= 1:
                if self.find_x_y_lower_layer(bin, bin.current_layer.previous_layer, package):
                    print('Package fits in layer below')
                    bin.current_layer.previous_layer.pack(package, self.x, self.y)
                    return False

        while(True):
            self.find_x_y(bin.current_layer, package)

            if self.package_fits:
                print('Package fits ')
                bin.current_layer.pack(package, self.x, self.y)
                return False
            else: #The package does not fit in the current layer
                print('Package does not fit in current layer')
                layer_success = bin.new_layer()
                if not layer_success:
                    print('No more layers available in bin')

                    if self.foreign == True:
                        self.current_bin_foreign = self.current_bin_foreign + 1
                    else:
                        self.current_bin = self.current_bin + 1

                    self.handle(package)
                    return True
        
    
    def find_bin(self, package):
        print('Find the correct bin for package {0}'.format(package))

        if self.foreign == False:
            if len(self.bins) > self.current_bin:
                return self.bins[self.current_bin]
            else:
                self.bins.append(Bin(self.bins[0].width, self.bins[0].length, self.bins[0].max_layers))
                self.bins[self.current_bin].bin_id = self.next_bin_id
                self.next_bin_id = self.next_bin_id + 1
                return self.bins[self.current_bin]
        else:
            if len(self.bins_foreign) > self.current_bin_foreign:
                return self.bins_foreign[self.current_bin_foreign]
            else:
                self.bins_foreign.append(Bin(self.bins_foreign[0].width, self.bins_foreign[0].length, self.bins_foreign[0].max_layers))
                self.bins_foreign[self.current_bin_foreign].bin_id = self.next_bin_id
                self.next_bin_id = self.next_bin_id + 1
                return self.bins_foreign[self.current_bin_foreign]

    def find_x_y(self, layer, package_to_pack):
        self.x = 0
        self.y = 0 
        self.propose_x_y(layer, package_to_pack) #Recursively propose x,y coordinates until a valid set is found

    def propose_x_y(self, layer, package_to_pack):
        #print('proposing ' + str(self.x) + ',' + str(self.y))
        for p in layer.packages:
            if p.position_is_taken(self.x, self.y):
                if self.y + p.width + package_to_pack.width > layer.width: #package_to_pack does not fit in current column
                    self.y = 0 
                    if self.x + layer.find_occupying_package(self.x, self.y).length + package_to_pack.length > layer.length:
                        self.package_fits = False #No further columns are available in layer
                        return
                    else:
                        self.x = self.x + layer.find_occupying_package(self.x, self.y).length #start new column
                        self.propose_x_y(layer, package_to_pack)
                        return
                else: #package_to_pack does fit further down in current column
                    self.y = self.y + p.width
                    self.propose_x_y(layer, package_to_pack)
                    return
        
        #This is the case when the position is not taken by any other package. No position that would go out of bounds is ever proposed
        #We can now check if the layers below the current position are packed to a certain degree
        gravity = self.check_gravity(layer, package_to_pack)

        if gravity >= 0.75:
            self.package_fits = True
        else:
            if self.x + 2 + package_to_pack.length > layer.length: #TODO: Softcode '2' as longest package in current column
                self.package_fits = False #No further columns are available in layer
                return
            else:
                self.x = self.x + 2 #start new column
                self.propose_x_y(layer, package_to_pack)
                return

    def check_gravity(self, layer, package_to_pack): #recursively check product of ratio of occupied coordinates below package proposal
        Total = 0
        Total_occupied = 0.0

        if layer.previous_layer is not None:
            for x_grav in range(self.x, self.x+package_to_pack.length):
                for y_grav in range(self.y, self.y+package_to_pack.width):
                    Total = Total + 1
                    occupying_package = layer.previous_layer.find_occupying_package(x_grav, y_grav) #Utilises short-circuiting below
                    if occupying_package is not None and (occupying_package.colour == 0 or occupying_package.colour == 2):
                        Total_occupied = Total_occupied + 1
                    elif occupying_package is not None and (occupying_package.colour == 1 or occupying_package.colour == 3):
                        Total_occupied = Total_occupied + 0.5 #Fragile cargo only has 0.5 stability contribution

            return (Total_occupied / Total) * self.check_gravity(layer.previous_layer, package_to_pack)
        else:
            return 1

    def find_x_y_lower_layer(self, bin, lower_layer, package_to_pack):
        self.x = 0
        self.y = 0
        self.propose_x_y(lower_layer, package_to_pack)
        if self.package_fits == True:
            return self.check_airspace(bin, package_to_pack)

        return False

    def check_airspace(self, bin, package_to_pack):
        for x_air in range(self.x, self.x+package_to_pack.length):
            for y_air in range(self.y, self.y+package_to_pack.width):
                if bin.current_layer.find_occupying_package(x_air, y_air) is not None: #There is a package in the airspace above
                    return False

        return True

    def set_meta_data(self, package):
        if package.colour == 0:
            self.foreign = False
            self.fragile = False
        elif package.colour == 1:
            self.foreign = True
            self.fragile = True
        elif package.colour == 2:
            self.foreign = True
            self.fragile = False
        elif package.colour == 3:
            self.foreign = False
            self.fragile = True
        else:
            raise InvalidArgError('Package had an undefined colour')

    def find_bin_id_containing_package(self, p):
        for bin in self.bins:
            for layer in bin.layers:
                for package in layer.packages:
                    if package is p:
                        return bin.bin_id
        for bin in self.bins_foreign:
            for layer in bin.layers:
                for package in layer.packages:
                    if package is p:
                        return bin.bin_id
        