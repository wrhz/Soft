import os
import sys
import subprocess
import platform
import shutil
from extract_zip import extract_zip
from get_files import get_files
from build_modules import build_modules

def build_windows(python_home, project_dir):
    try:
        exe_path = os.path.join(project_dir, "build", "windows", "main.exe")
        exe_dir = os.path.dirname(exe_path)
        cpp_dir = os.path.join(project_dir, "windows")

        files = {
            *get_files(os.path.join(project_dir, "windows"), remove_files=["main.cpp"]),
            *get_files(os.path.join(project_dir, "src", "cpp"))
        }

        includePaths = [
            os.path.join(project_dir, "packages", "windows", "pybind11", "include"),
            os.path.join(python_home, "include"),
            os.path.join(project_dir, "src", "cpp")
        ]

        libPaths = [
            os.path.join(python_home, "libs")
        ]

        libFiles = [
            "user32.lib",
            "gdi32.lib",
            "shlwapi.lib",
            f"python{platform.python_version().split('.')[0]}{platform.python_version().split('.')[1]}.lib"
        ]

        cmd = [
            "cl",
            "/EHsc",
            "/std:c++17",
            "/Zi",
            f"/Fe:{os.path.join(project_dir, "build", "windows", "main.exe")}",
            f"/Fd:{os.path.join(project_dir, "build", "windows", "main.pdb")}",
            "main.cpp"
        ]

        if not os.path.exists(os.path.join(project_dir, "build", "windows")):
            os.makedirs(os.path.join(project_dir, "build", "windows"))
        else:
            for file in os.listdir(os.path.join(project_dir, "build", "windows")):
                if file in ["python"]:
                    continue

                if os.path.isfile(os.path.join(project_dir, "build", "windows", file)):
                    os.remove(os.path.join(project_dir, "build", "windows", file))
                else:
                    shutil.rmtree(os.path.join(project_dir, "build", "windows", file))

        if len(files) > 0:
            for file in files:
                cmd.append(file)
        if len(includePaths) > 0:
            for includePath in includePaths:
                cmd.append(f"/I{includePath}")
        cmd.append("/link")
        if len(libPaths) > 0:
            for libPath in libPaths:
                cmd.append(f"/LIBPATH:{libPath}")
        if len(libFiles) > 0:
            for libFile in libFiles:
                cmd.append(libFile)

        if not os.path.exists(exe_dir + "\\python"):
            extract_zip(os.path.join(project_dir, "python", "windows-python.zip"), exe_dir)
            subprocess.run([os.path.join(exe_dir, "python", "python.exe"), "get-pip.py"], cwd=os.path.join(exe_dir, "python"))
        
        shutil.copytree(os.path.join(project_dir, "config"), os.path.join(exe_dir, "config"))
        shutil.copytree(os.path.join(project_dir, "resources"), os.path.join(exe_dir, "resources"))
        shutil.copytree(os.path.join(project_dir, "packages", "windows"), os.path.join(exe_dir, "packages"))
        for dir in os.listdir(os.path.join(project_dir, "src", "python")):
            build_modules(os.path.join(project_dir, "src", "python", dir), os.path.join(exe_dir, "src"), "python")
        build_modules(os.path.join(project_dir, "lib"), exe_dir, "python")

        subprocess.run(cmd, cwd=cpp_dir)
        if sys.argv[1] == "run":
            subprocess.run([exe_path], cwd=exe_dir)
    except Exception as e:
        print(e)