class Package:
    def __init__(self, width, length, colour = 1):
        self.width = width
        self.length = length
        #self.height = height
        #self.is_fragile = is_fragile
        #self.destination = destination
        self.position = None
        self.colour = colour

    def position_is_taken(self, x, y):
        #Assumes position is initialized and test proposed position of new package
        if self.position != None:
            return (self.position.x <= x and x < self.position.x + self.length) and (self.position.y <= y and y < self.position.y + self.width)
        else:
            return False

    def __str__(self):
        return 'P[{0}x{1}]'.format(self.width, self.length)
