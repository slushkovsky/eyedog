from os.path import join, dirname, realpath, normpath
from os import environ

PROJ_ROOT = normpath(join(dirname(realpath(__file__)), '../'))

def butify_path(path): 
	return path.replace(environ['HOME'], '~')

def near_module(module, path): 
    return normpath(join(dirname(realpath(__import__('visual.' + module).__file__)), path))

def near_file(file, path):
	return normpath(join(dirname(realpath(file)), path)) 
