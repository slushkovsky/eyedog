import os
import cv2

import cfg

os.environ["GLOG_minloglevel"] = "2"
from ._classifier import Classifier


class FaceClassifier(object):
	def __init__(self): 
		self.classifiers = {}
		self.transformers = {}

		for name, params in cfg.CLASSIFIERS.items(): 
			self.classifiers[name] = Classifier(params.struct, params.model, params.mean_data)
			self.transformers[name] = {
				'size': params.input_shape[-2:],
				'func': params.transform
			}

	def recognize(self, face):
		result = {}

		for name, classifier in self.classifiers.items():
			photo = cv2.resize(face.photo, self.transformers[name]['size'])
			photo = self.transformers[name]['func'](photo)

			net_out = classifier.predict(photo)

			if cfg.ENABLE_GENDER_CRUTCH: 
				if name == 'gender': 
					if net_out[0] > 0.999: 
						net_out = [0, 1.0]


			result[name] = net_out # Index of max value on softmax layer

		return result