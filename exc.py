class CaptureError(Exception):
	def __str__(self):
		return "[Internal] " + super(CaptureError, self).__str__()

class FileNotFoundError(Exception):
    def __str__(self):
        return "File '%s' not found" % super(FileNotFoundError, self).__str__()

class ConfigError(Exception):
    def __str__(self): 
        return "Config ERROR: " + super(ConfigError, self).__str__()

class PyBoostError(FileNotFoundError): 
	pass

class ConfigDefaultsError(Exception):
    def __str__(self): 
        return "[Internal] Config ERROR: Missed required config value '%s' (why not default value?)" % super(ConfigDefaultsError, self).__str__()

