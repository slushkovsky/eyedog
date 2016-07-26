import cv2

from .utils.img import gray, roi
from .utils.types import Rect
from .faces_base import Face
from . import cfg

class FacesDetector(object): 
    def __init__(self):
        self.cascade = cv2.CascadeClassifier()
        ret = self.cascade.load(cfg.CASCADE_FILE)

        if not ret: 
            raise FileNotFoundError('Couldn\'t open cascade \'{}\''.format(cfg.CASCADE_FILE))

    def find_faces(self, frame):
        detects = self.cascade.detectMultiScale(gray(frame), **cfg.CASCADE_PARAMS)

        return [Face(frame, rect) for rect in [Rect(*d) for d in detects]]