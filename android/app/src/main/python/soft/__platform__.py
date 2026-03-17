import sys,platform
def detect_platform():
	B='android'
	if'pyodide'in sys.modules or platform.system()=='Emscripten':return'web'
	if sys.platform==B or platform.system()=='Android':return B
	A=platform.system()
	if A=='Windows':return'windows'
	elif A=='Linux':return'linux'
	return'unknown'