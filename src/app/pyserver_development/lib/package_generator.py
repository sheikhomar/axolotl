import random
from lib import Package

class PackageGenerator:
    def __init__(self):
        ###Random colour code from internet###
        self.weighted_choices = [(0, 5), (2, 2), (3, 2), (1, 1)] #50 % red, 20% blue, 20% yellow and 10% green
        self.population = [val for val, cnt in weighted_choices for i in range(cnt)]

    def generate(self):
        p = Package(width=random.randrange(4)+1, length=2, colour=random.choice(population)) 


