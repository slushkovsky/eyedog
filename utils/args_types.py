import os
import re
from argparse import ArgumentError

from .types import Size

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