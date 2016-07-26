import cv2
from datetime import datetime
import numpy as np

from utils.types import Rect
from utils.img import roi
from utils import vec_diff_len
import tracking
import cfg


FACE_ACTIVE = 'active'
FACE_LOSSED = 'lossed'
FACE_CAPTURED = 'captrued'
FACE_TRACKED = 'tracked'


class Face(Rect):
    def __init__(self, frame, rect):
        super().__init__(rect.x, rect.y, rect.width, rect.height) 
        self.photo = roi(frame, rect) 
        self.appear_time = datetime.now()
        self.loss_time = None

        self.predictions = {}

        self.tracker = tracking.Tracker(*cfg.TRACKER_TERM_CRITERIA)
        self.tracker.capture(frame, (rect.x, rect.y, rect.width, rect.height))


    def __eq__(self, other): 
        area_relation = self.area() / float(other.area())
        dist = vec_diff_len(self.tl(), other.tl())
        corr = cv2.matchTemplate(self.photo, cv2.resize(other.photo, self.photo.shape[:2][::-1]), 
            cfg.EQ_FACES_TM_METHOD)[0][0]

        return not (area_relation < cfg.EQ_FACES_MIN_AREA_REL or
                    dist > self.diagonal_len() * cfg.EQ_FACES_MIN_DIST_COEFF or
                    corr < cfg.EQ_FACES_MIN_CORR)

    def dump(self):
        predictions = {}

        for name, preds in self.predictions.items():
            predictions[name] = cfg.CLASSIFIERS[name].labels[self.__accumulate_hisory(preds)]

        loss_time = None

        if self.loss_time is not None: 
            loss_time = self.loss_time.strftime("%H:%M:%S")

        return {
            'appear_time': self.appear_time.strftime("%H:%M:%S"), 
            'loss_time': loss_time,
            'rect': (self.x, self.y, self.width, self.height),
            'predictions': predictions
        }

    def merge(self, other): 
        for predict_name in other.predictions: 
            self.predictions[predict_name] += other.predictions[predict_name]
        
        self.appear_time = min(self.appear_time, other.appear_time)

    def track(self, frame): 
        res = self.tracker.track(frame)

        if res is not None: 
            self.x = res[0]
            self.y = res[1]
            self.width = res[2]
            self.height = res[3]

        
        return res

    def update(self, photo, box): 
        self.photo = photo
        self.rect = box
        # Tracker recapture? 

    def append_predictions(self, preds): 
        for name, pred in preds.items():
            self.predictions[name] = self.predictions.get(name, []) + [pred]
              

    @staticmethod
    def __accumulate_hisory(history): 
        accumulated = [sum(vals) for vals in zip(*history)]
        
        return np.array(accumulated).argmax()



class FacesBase(dict): 
    def __init__(self):
        self[FACE_ACTIVE] = {FACE_CAPTURED: [], FACE_TRACKED:[]}
        self[FACE_LOSSED] = []

    def update(self, faces): 
        for f in faces: 
            status, index = self.__find_similar_face(f)

            if status is None: 
                self[FACE_ACTIVE][FACE_CAPTURED].append(f)
            else: 
                self[FACE_ACTIVE][status][index].update(f.photo, Rect(f.x, f.y, f.width, f.height))

    def dump(self):
        return [f.dump() for f in self[FACE_ACTIVE][FACE_CAPTURED] + 
                                  self[FACE_ACTIVE][FACE_TRACKED]]

    def track(self, frame): 
        self[FACE_ACTIVE][FACE_TRACKED] += self[FACE_ACTIVE][FACE_CAPTURED]
        self[FACE_ACTIVE][FACE_CAPTURED] = []

        for index, face in enumerate(self[FACE_ACTIVE][FACE_TRACKED]): 
            result = face.track(frame)

            if result is None:
                face.loss_time = datetime.utcnow()
                self[FACE_LOSSED].append(face)

                del self[FACE_ACTIVE][FACE_TRACKED][index]

        if cfg.MERGE_INTERSECTED_FACES: 
            self.__merge_intersected()
            

    def __find_similar_face(self, face):
        for status, faces in self[FACE_ACTIVE].items():
            for index, f in enumerate(faces): 
                if f == face: 
                    return (status, index)

        return (None, None)


    def __merge_intersected(self): 
        for status, faces in self[FACE_ACTIVE].items():
            for i, face in enumerate(faces): 
                for other_i, other_face in enumerate(faces[i + 1:]): 
                    if face.area() != 0: #?
                        inter = Rect.intersection(face, other_face)

                        if inter / float(face.area()) > cfg.FACES_MAX_INTERSECTION: 
                            self[FACE_ACTIVE][status][i].merge(other_face)
                            del self[FACE_ACTIVE][status][other_i]




