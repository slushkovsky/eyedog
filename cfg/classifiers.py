import cv2
from collections import namedtuple

from .resources import __resource

ClassifierDescr = namedtuple('ClassifierDescr', ('struct', 'model', 'mean_data', 'input_shape', 
	                                             'transform', 'labels'))


CLASSIFIERS = {
	'gender': ClassifierDescr(
			struct=__resource('cnn/gender.prototxt'), 
			model=__resource('cnn/gender.caffemodel'), 
			mean_data=__resource('cnn/gender_mean.binaryproto'), 
			input_shape=(1, 1, 50, 50),
			transform=lambda img: cv2.cvtColor(img, cv2.COLOR_BGR2GRAY),
			labels={
				0: 'female', 
				1: 'male'
			}
		),
	'age': ClassifierDescr(
			struct=__resource('cnn/age.prototxt'),    
			model=__resource('cnn/age.caffemodel'),
			mean_data=__resource('cnn/age_mean.binaryproto'),    
			input_shape=(1, 1, 50, 50),
			transform=lambda img: cv2.cvtColor(img, cv2.COLOR_BGR2GRAY),
			labels={
				0: "15-20", 
				1: "20-25", 
				2: "25-30",
				3: "30-40", 
				4: "40-50", 
				5: "50-60"
			}
		)
}

ENABLE_GENDER_CRUTCH = True