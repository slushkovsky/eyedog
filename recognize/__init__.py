import os
import cv2
from glob import glob

from ..utils.pyboost import import_module
from ..utils.path import near_file, TARGETING_ROOT
from .. import cfg

os.environ["GLOG_minloglevel"] = "2"

_classifier = import_module('visual.recognize._classifier',
						   sources=glob(near_file(__file__, 'src') + '/*'), 
						   libs=['caffe', 'proto', 'pyboost_opencv'],
						   include_paths=[near_file(__file__, 'include'),
						    			  os.path.join(TARGETING_ROOT, 'visual'),
						                  '/usr/include'],
						   libs_paths=['/usr/lib/x86_64-linux-gnu/', '/usr/local/lib'])


Classifier = _classifier.Classifier


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