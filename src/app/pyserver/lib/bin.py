from .invalid_arg_error import *
from .layer import *

class Bin:
    def __init__(self, width, length, max_layers):
        self.width = width
        self.length = length
        self.max_layers = max_layers
        self.current_layer = Layer(width=width, length=length, previous_layer=None)
        self.layers = [self.current_layer]

    # Closes the current layer and opens new layer in the bin.
    def new_layer(self):
        if len(self.layers) >= self.max_layers:
            return False
        else:
            self.current_layer = Layer(width=self.width, length=self.length, previous_layer=self.current_layer)
            self.layers.append(self.current_layer)
            return True