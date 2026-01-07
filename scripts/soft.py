import os
import sys
from build_windows import build_windows
from build_linux import build_linux

if len(sys.argv) < 3:
    print("Usage: soft.py <run|build> <windows|linux|android|web>")
    exit(1)

if sys.argv[1] != "run" and sys.argv[1] != "build":
    print(f"Invalid argument: {sys.argv[1]}")
    exit(1)

python_home = os.environ.get("PYTHON_HOME")
if python_home is None and sys.argv[2] == "windows":
    print("PYTHON_HOME is not set")
    exit(1)

object_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if sys.argv[2] == "windows":
    build_windows(python_home, object_dir)
elif sys.argv[2] == "linux":
    build_linux(object_dir)
else:
    print(f"Invalid argument: {sys.argv[2]}")
    exit(1)