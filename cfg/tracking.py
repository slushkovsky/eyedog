from collections import namedtuple

__TermCriteria = namedtuple('TrackTermCriteria', ('max_confidence_eps', 
	                                              'obj_min_width', 'obj_min_height'))

TRACKER_TERM_CRITERIA = __TermCriteria(0.1, 30, 30)