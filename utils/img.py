import cv2

def roi(frame, rect):
    return frame[rect.y : rect.y + rect.height, rect.x : rect.x + rect.width]

def gray(frame):
    if len(frame.shape) == 3:
        return cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    return frame