from .invalid_arg_error import *
from .layer import *

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