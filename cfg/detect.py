from .resources import __resource

FACES_DETECT_PERIOD = 10
CASCADE_FILE = __resource('cascades/haarcascade_frontalface_alt.xml')
CASCADE_PARAMS = dict(scaleFactor=1.2, minNeighbors=5, minSize=(30, 30), maxSize=(180, 180))
