class Position:
    pass

class PackedPosition(Position):
    def __init__(self, x, y):
        self.x = x
        self.y = y

class ConveyorPosition(Position):
    def __init(self, time):
        self.time = time
