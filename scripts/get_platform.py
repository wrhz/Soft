import sys

def get_platform():
    if sys.platform == "win32":
        return "windows"
    elif sys.platform == "linux":
        return "linux"
    else:
        raise ValueError(f"Unsupported platform: {sys.platform}")
