class Position:
    pass

class PackedPosition(Position):
    def __init__(self, x, y, layer):
        self.x = x
        self.y = y
        self.layer = layer
