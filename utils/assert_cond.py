from collections import Iterable

def is_listed_type(value): 
	return isinstance(value, Iterable) and not isinstance(value, str)