import cv2

from faces_base import FacesBase, FACE_ACTIVE, FACE_CAPTURED
from detect import FacesDetector
from recognize import FaceClassifier
import cfg
import exc

class CaptureApp(object): 
    def __init__(self, capture, resize_to, output):
        self.__output = output 
        self.__capture = cv2.VideoCapture(capture)
        self.__resize_to = resize_to

    def on_frame(self, frame): 
        raise NotImplementedError()

    def run(self): 
        while True: 
            ret, frame = self.__capture.read() 

            if not ret: 
                raise exc.CaptureError('Captured empty frame')

            if self.__resize_to is not None: 
                frame = cv2.resize(frame, self.__resize_to)

            self.on_frame(frame)


    def show_frame(self, frame): 
        self.__output.write(frame)


    def run_single(self): 
        ret, frame = self.__capture.read() 

        if not ret: 
            raise exc.CaptureError('Captured empty frame')

        if self.__resize_to is not None: 
            frame = cv2.resize(frame, self.__resize_to)

        self.on_frame(frame)


class App(CaptureApp):
    def __init__(self, capture, resize_to, output):
        super().__init__(capture, resize_to, output)

        self.faces = FacesBase()
        self.face_detector = FacesDetector()
        self.recognizer = FaceClassifier()

        self.active_faces = []


    def on_frame(self, frame):
        self.faces.update(self.face_detector.find_faces(frame))
        self.__recognize_faces()
        self.faces.track(frame)

        self.active_faces = []

        for status, faces in self.faces[FACE_ACTIVE].items(): 
            for f in faces: 
                self.active_faces.append(f)

        if cfg.__dict__.get('DEBUG', False): 
            self.__show_result(frame)


    def __show_result(self, frame): 
        draw_frame = frame[:]

        for status, faces in self.faces[FACE_ACTIVE].items(): 
            for face in (f.dump() for f in faces):
                is_male = face['predictions']['gender'] == 'male'
                color = (255, 0, 0) if is_male else (0, 0, 255)

                tl = (face['rect'][0], face['rect'][1]) 
                br = (face['rect'][0] + face['rect'][2],
                      face['rect'][1] + face['rect'][3])

                cv2.rectangle(draw_frame, tl, br, color, 2)

        self.show_frame(frame)
        

    def __recognize_faces(self):
        for face in self.faces[FACE_ACTIVE][FACE_CAPTURED]:  
            face.append_predictions(self.recognizer.recognize(face))