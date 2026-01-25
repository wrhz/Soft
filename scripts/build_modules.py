import os
import subprocess
import shutil

def build_modules(dir_path, exe_dir, python_exe):
    output_path = os.path.join(exe_dir, os.path.basename(dir_path))
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    for module in os.listdir(dir_path):
        path = os.path.join(dir_path, module)
        if os.path.isdir(path):
            build_modules(path, output_path, python_exe)
        elif module.endswith(".py"):
            subprocess.run([python_exe, "-m", "python_minifier", path, "--output", os.path.join(output_path, module)], cwd=exe_dir)
            subprocess.run([python_exe, "-m", "compileall", "-b", os.path.join(output_path, module)], cwd=exe_dir)
            os.remove(os.path.join(output_path, module))
        else:
            shutil.copy(path, exe_dir)
