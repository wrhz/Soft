import os
import sys
import subprocess
import platform
import shutil
from get_files import get_files
from build_modules import build_modules

def build_linux(project_dir):
    try:
        x86_64_path = os.path.join(project_dir, "build", "linux", "app")
        x86_64_dir = os.path.dirname(x86_64_path)

        if not os.path.exists(project_dir + "/build/linux"):
            os.makedirs(project_dir + "/build/linux")
        else:
            for file in os.listdir(os.path.join(project_dir, "build", "linux")):
                if os.path.isdir(os.path.join(project_dir, "build", "linux", file)):
                    shutil.rmtree(os.path.join(project_dir, "build", "linux", file))
                else:
                    os.remove(os.path.join(project_dir, "build", "linux", file))

        shutil.copytree(os.path.join(project_dir, "config"), os.path.join(x86_64_dir, "config"))
        shutil.copytree(os.path.join(project_dir, "resources"), os.path.join(x86_64_dir, "resources"))

        for dir in os.listdir(os.path.join(project_dir, "src", "python")):
            build_modules(os.path.join(project_dir, "src", "python", dir), os.path.join(x86_64_dir, "src"), "python3")
        build_modules(os.path.join(project_dir, "lib"), x86_64_dir, "python3")

        subprocess.run(["cmake", "../..", "-G", "Ninja"], cwd=x86_64_dir)
        subprocess.run(["ninja"], cwd=x86_64_dir)

        if sys.argv[1] == "run":
            subprocess.run([x86_64_path], cwd=x86_64_dir)
    except Exception as e:
        print(f"Error: {e}")