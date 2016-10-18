class PackingAdvisor:
    def __init__(self, bins = []):
        self.bins = bins
        self.x = 0
        self.y = 0
        self.package_fits = False

    def handle(self, package):
        
        bin = self.find_bin(package) #ATM just returns bin[0]
        
        while(True):
            self.find_x_y(bin.current_layer, package)

            if self.package_fits:
                print('Package fits ')
                bin.current_layer.pack(package, self.x, self.y)
                return
            else:
                print('Package does not fit in current layer')
                bin.new_layer()
        
    
    def find_bin(self, package):
        print('Find the correct bin for package {0}'.format(package))
        return self.bins[0]

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
        
        self.package_fits = True
        return #This is the case when the position is not taken by any other package. No position that would go out of bounds is ever proposed