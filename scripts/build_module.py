import os
import re

def extract_names(file_path):
    with open(file_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            match = re.match(r'^([A-Za-z0-9._-]+)', line)
            if match:
                print(match.group(1))

def build_module(python_home: str, dir: str, project_dir: str):
    for file in os.listdir(dir):
        if os.path.isfile(os.path.join(dir, file)):
            file_path = os.path.join(dir, file)
            print(file_path)
        else:
            build_module(python_home, os.path.join(dir, file), project_dir)
