from lib import Bin
from lib import InvalidArgError

class PackingAdvisor:
    def __init__(self, bin):
        #Initialise internal bin-lists NOTE: two lists of bins - one foreign and one non-foreign
        bin.bin_id = 1
        bin.foreign = False
        self.bins = [bin]
        bin2 = Bin(bin.width, bin.length, bin.max_layers)
        bin2.bin_id = 2
        bin2.foreign = True
        self.bins_foreign = [bin2]

        # Internal global variables. Strict rules for which methods may assign to these
        self.x = 0
        self.y = 0
        self.package_fits = False
        self.current_bin = 0
        self.current_bin_foreign = 0
        self.next_bin_id = 3

        # Package metadata
        self.fragile = False #Currently obsolete
        self.foreign = False

        self.COLUMN_LENGTH = 2

    def handle(self, package): #Packs input package and returns whether a new bin was opened or not

        self.set_meta_data(package)
        
        bin = self.find_bin(package)
        
        if self.try_pack_lower_layer(bin=bin, package_to_pack=package):
            return False #Package was packed in lower layer (and thus no new bin was opened)

        while(True): #While loop iterates for each layer in bin
            self.update_x_y(bin.current_layer, package) #Sets globals x, y and package_fits

            if self.package_fits: #Package fits at proposed x,y
                print('Package fits ')
                bin.current_layer.pack(package, self.x, self.y)
                return False
            else: #The package does not fit in the current layer - open a new one
                print('Package does not fit in current layer')
                layer_success = bin.new_layer()
                if not layer_success: #No more layers available, open new bin (foreign or non-foreign) and recall handle
                    print('No more layers available in bin')

                    if self.foreign == True:
                        self.current_bin_foreign = self.current_bin_foreign + 1
                    else:
                        self.current_bin = self.current_bin + 1

                    self.handle(package)
                    return True #Return true, as new bin was opened.
        
    
    def find_bin(self, package): #Returns current foreign or non-foreign bin based on package and current values of globals.
        print('Find the correct bin for package {0}'.format(package))

        if (package.length > self.bins[0].length or package.width > self.bins[0].width):
            raise InvalidArgError('Package will never fit in any bin')

        if self.foreign == False:
            if len(self.bins) > self.current_bin:
                return self.bins[self.current_bin]
            else:
                self.bins.append(Bin(self.bins[0].width, self.bins[0].length, self.bins[0].max_layers, False))
                self.bins[self.current_bin].bin_id = self.next_bin_id
                self.next_bin_id = self.next_bin_id + 1
                return self.bins[self.current_bin]
        else:
            if len(self.bins_foreign) > self.current_bin_foreign:
                return self.bins_foreign[self.current_bin_foreign]
            else:
                self.bins_foreign.append(Bin(self.bins_foreign[0].width, self.bins_foreign[0].length, self.bins_foreign[0].max_layers, True))
                self.bins_foreign[self.current_bin_foreign].bin_id = self.next_bin_id
                self.next_bin_id = self.next_bin_id + 1
                return self.bins_foreign[self.current_bin_foreign]

    def update_x_y(self, layer, package_to_pack):
        self.x = 0
        self.y = 0 
        self.propose_x_y(layer, package_to_pack) #Recursively propose x,y coordinates until a valid set is found

    def propose_x_y(self, layer, package_to_pack): #Once entire recursive stack returns, x,y and package_fits are set
        
        #Recursively call this function until unoccupied position is proposed.
        for p in layer.packages:
            if p.position_is_taken(self.x, self.y): 
                return self.propose_new_x_y(p, layer, package_to_pack) #Position is occupied and new x,y's are proposed. Might call propose_x_y
        

        #Position is not occupied. No position that would go out of bounds is ever proposed at this point
        #We can now check if the layers below the current position can support the current package
        gravity = self.calc_gravity(layer, package_to_pack)

        return self.check_gravity(gravity, layer, package_to_pack)

    def propose_new_x_y(self, p_occupying, layer, package_to_pack): #Sets new x,y and calls propose_x_y again (or returns if no place in layer)
        if self.y + p_occupying.width + package_to_pack.width > layer.width: #package_to_pack does not fit in current column
            self.y = 0 
            if self.x + self.COLUMN_LENGTH + package_to_pack.length > layer.length:
                self.package_fits = False #No further columns are available in layer
                return
            else:
                self.x = self.x + self.COLUMN_LENGTH #start new column
                self.propose_x_y(layer, package_to_pack)
                return
        else: #package_to_pack does fit further down in current column
            self.y = self.y + p_occupying.width
            self.propose_x_y(layer, package_to_pack)
            return

    def calc_gravity(self, layer, package_to_pack): #recursively check product of ratio of occupied coordinates below package proposal
        Total = 0
        Total_occupied = 0.0

        if layer.previous_layer is not None: #Sum occupied and total coordinates in previous layer
            for x_grav in range(self.x, self.x+package_to_pack.length):
                for y_grav in range(self.y, self.y+package_to_pack.width):
                    Total = Total + 1
                    occupying_package = layer.previous_layer.find_occupying_package(x_grav, y_grav) #Utilises short-circuiting below
                    if occupying_package is not None and (occupying_package.is_fragile == False):
                        Total_occupied = Total_occupied + 1 
                    elif occupying_package is not None and (occupying_package.is_fragile == True):
                        Total_occupied = Total_occupied + 0.5 #Fragile cargo only has 0.5 stability contribution
            return (Total_occupied / Total) * self.calc_gravity(layer.previous_layer, package_to_pack)
        else:
            return 1
    
    def check_gravity(self, gravity, layer, package_to_pack):
        if gravity >= 0.75:
            self.package_fits = True
            return
        else: #Gravitational support too low: Try another column
            if self.x + self.COLUMN_LENGTH + package_to_pack.length > layer.length:
                self.package_fits = False #No further columns are available in layer
                return
            else:
                self.y = 0
                self.x = self.x + self.COLUMN_LENGTH #start new column
                self.propose_x_y(layer, package_to_pack)
                return
    
    def try_pack_lower_layer(self, bin, package_to_pack): #Attempts to pack in layer below current - returns true if Succesfull
        if bin.current_layer.previous_layer is not None:
            if (bin.current_layer.previous_layer.calc_fill_level() + ((package_to_pack.length * package_to_pack.width) / (bin.width * bin.length))) <= 1:
                if self.update_x_y_lower_layer(bin, bin.current_layer.previous_layer, package_to_pack): #True is returned if it fits and x,y is set
                    print('Package fits in layer below')
                    bin.current_layer.previous_layer.pack(package_to_pack, self.x, self.y)
                    return True

        return False

    def update_x_y_lower_layer(self, bin, lower_layer, package_to_pack): #Includes an airspace check unlike update_x_y
        self.x = 0
        self.y = 0
        self.propose_x_y(lower_layer, package_to_pack)
        if self.package_fits == True:
            return self.check_airspace(bin, package_to_pack)

        return False

    #Checks if x,y coordinates are available for package in current layer. Tested x,y were proposed in layer below.
    def check_airspace(self, bin, package_to_pack):
        for x_air in range(self.x, self.x+package_to_pack.length):
            for y_air in range(self.y, self.y+package_to_pack.width):
                if bin.current_layer.find_occupying_package(x_air, y_air) is not None: #There is a package in the airspace above
                    return False

        return True

    #Simple switch method to set meta-data necessary for handling
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

    #Method called outside of class to retrieve Bin object if given Package
    def find_bin_containing_package(self, p):
        for bin in self.bins:
            for layer in bin.layers:
                for package in layer.packages:
                    if package is p:
                        return bin
        for bin in self.bins_foreign:
            for layer in bin.layers:
                for package in layer.packages:
                    if package is p:
                        return bin
        