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

project_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if sys.argv[2] == "windows":
    python_home = os.path.join(project_dir, "build", "windows", "python")
    build_windows(python_home, project_dir)
elif sys.argv[2] == "linux":
    build_linux(project_dir)
else:
    print(f"Invalid argument: {sys.argv[2]}")
    exit(1)