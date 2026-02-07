import os
import subprocess
import shutil
from build_modules import build_modules
from get_platform import get_platform

def build_android(project_dir):
    if get_platform() == "linux":
        python = "python3"
    else:
        python = "python"

    not_remove_files_and_dirs = [
        "app.py"
    ]

    android_python_dir = os.path.join(project_dir, "android", "app", "src", "main", "python")

    for file in os.listdir(android_python_dir):
        if file not in not_remove_files_and_dirs:
            if os.path.isfile(os.path.join(android_python_dir, file)):
                os.remove(os.path.join(android_python_dir, file))
            else:
                shutil.rmtree(os.path.join(android_python_dir, file))

    build_modules(os.path.join(project_dir, "src", "python"), os.path.dirname(android_python_dir), python)
    build_modules(os.path.join(project_dir, "lib"), android_python_dir, python)
