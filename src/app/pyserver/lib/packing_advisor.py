class PackingAdvisor:
    def __init__(self, bins = []):
        self.bins = bins
        self.x = 0
        self.y = 0
        self.package_fits = False
        self.current_bin = 0

    def handle(self, package):
        
        bin = self.find_bin(package)
        
        while(True):
            self.find_x_y(bin.current_layer, package)

            if self.package_fits:
                print('Package fits ')
                bin.current_layer.pack(package, self.x, self.y)
                return
            else: #The package does not fit in the current layer
                print('Package does not fit in current layer')
                layer_success = bin.new_layer()
                if not layer_success:
                    print('No more layers available in bin')
                    self.current_bin = self.current_bin + 1
                    self.handle(package)
                    return
        
    
    def find_bin(self, package):
        print('Find the correct bin for package {0}'.format(package))
        if len(self.bins) > self.current_bin:
            return self.bins[self.current_bin]
        else:
            raise EOFError('NO MORE BINS AVAILABE')

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

        if gravity > 0.75:
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
        Total_occupied = 0

        if layer.previous_layer is not None:
            for x_grav in range(self.x, self.x+package_to_pack.length):
                for y_grav in range(self.y, self.y+package_to_pack.width):
                    Total = Total + 1
                    if layer.previous_layer.find_occupying_package(x_grav, y_grav) is not None:
                        Total_occupied = Total_occupied + 1

            return (Total_occupied / Total) * self.check_gravity(layer.previous_layer, package_to_pack)
        else:
            return 1