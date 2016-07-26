import os
from glob import glob

from utils.pyboost import import_module


CURRENT_DIR = os.path.dirname(os.path.realpath(__file__))


_tracking = import_module('tracking._tracking',
						  sources=glob(os.path.join(CURRENT_DIR, 'src') + '/*'), 
						  libs=[
						      'pyboost_opencv', 
						      'opencv_core', 
						      'opencv_videoio',  
						      'opencv_video',
						      'opencv_imgproc'
						  ],
						  include_paths=[
						      os.path.join(CURRENT_DIR, 'include')
						  ])

Tracker = _tracking.TemplateTracker