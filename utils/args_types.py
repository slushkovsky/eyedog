import os
import re
import cv2
from argparse import ArgumentError

from .types import Size, ScreenOutput

def video_output(value, create_if_not_exists=True): 
	if value == 'screen': 
		return ScreenOutput()

	writer = cv2.VideoWriter(value, cv2.VideoWriter_fourcc(*'XVID'), 10.0, (1080, 720))

	if not writer.isOpened(): 
		raise ArgumentError('Couldn\'t create video writer (file: {!r}}'.format(value))

	return writer


def capture(value): 
	try: 
		return int(value)
	except ValueError: 
		if not os.path.exists(value): 
			raise ArgumentError('Capture {!r} not exists'.format(value))
		return value


def size(value): 
	SIZE_RE = re.compile('(?P<width>\d+)x(?P<height>\d+)');

	m = SIZE_RE.match(value)

	if m is None: 
		raise ArgumentError('Unsupported size argument format: must be WxH where W - width (int), H - height (int)')

	return Size(width =int(m.group('width')),
		        height=int(m.group('height')))