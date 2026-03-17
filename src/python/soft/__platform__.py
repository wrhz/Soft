import sys
import platform

def detect_platform():
    if 'pyodide' in sys.modules or platform.system() == 'Emscripten':
        return 'web'

    if sys.platform == 'android' or platform.system() == 'Android':
        return 'android'

    system_name = platform.system()
    if system_name == 'Windows':
        return 'windows'
    elif system_name == 'Linux':
        return 'linux'
    
    return 'unknown'
