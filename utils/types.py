import numpy as np
from collections import namedtuple

Point = namedtuple('Point', ('x', 'y'))
Size = namedtuple('Size', ('width', 'height'))

class Rect(object):
    def __init__(self, x, y, width, height): 
        self.x = int(x)
        self.y = int(y)
        self.width = int(width)
        self.height = int(height)

    @classmethod
    def intersection(cls, r1, r2):
        inter_tl = Point(max(r1.tl().x, r2.tl().x), max(r1.tl().y, r2.tl().y))
        inter_br = Point(min(r1.br().x, r2.br().x), min(r1.br().y, r2.br().y))

        if inter_tl.x >= inter_br.x or inter_tl.y >= inter_br.y: 
            return 0 
            
        return abs(inter_tl.x - inter_br.x) * abs(inter_tl.y - inter_br.y)

    def center(self):
        return Point(int((self.tl().x + self.br().x)/2), int((self.tl().y + self.br().y )/2))

    def br(self):
        return Point(self.x + self.width, self.y + self.height)

    def tl(self):
        return Point(self.x, self.y) 

    def diagonal_len(self):
        return (self.width ** 2 + self.height ** 2) ** 0.5

    def area(self):
        return self.width * self.height

    def clip_to_area(self, area_width, area_height):
        self.x = np.clip(self.x, 0, area_width)
        self.y = np.clip(self.y, 0, area_height)

        if self.x + self.width > area_width:
            self.width = area_width - self.x

        if self.y + self.height > area_height:
            self.height = area_height - self.y

    def increase(self, scale):
        size_scale = 1 + 2.0 / scale

        self.x -= int(self.width / scale)
        self.y -= int(self.height / scale)

        self.width *= size_scale
        self.height *= size_scale