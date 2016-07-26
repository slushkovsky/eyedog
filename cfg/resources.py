import os

RESOURCES_FOLDER = os.path.join(os.path.dirname(os.path.realpath(__file__)), '../resources/')
__resource = lambda path: os.path.join(RESOURCES_FOLDER, path) 
