import os
import sys
import subprocess
import platform

def build_windows(python_home, object_dir):
    try:
        exe_path = os.path.join(object_dir, "build", "windows", "main.exe")
        exe_dir = os.path.dirname(exe_path)
        cpp_dir = os.path.join(object_dir, "windows")

        files = [
            
        ]

        includePaths = [
            f"{python_home}\\Lib\\site-packages\\pybind11\\include",
            f"{python_home}\\include"
        ]

        libPaths = [
            f"{python_home}/libs"
        ]

        libFiles = [
            "user32.lib",
            "gdi32.lib",
            f"python{platform.python_version().split('.')[0]}{platform.python_version().split('.')[1]}.lib"
        ]

        cmd = [
            "cl",
            "/EHsc",
            "/std:c++17",
            "/Zi",
            f"/Fe:{os.path.join(object_dir, "build", "windows", "main.exe")}",
            f"/Fo:{os.path.join(object_dir, "build", "windows", "main.obj")}",
            f"/Fd:{os.path.join(object_dir, "build", "windows", "main.pdb")}",
            "main.cpp"
        ]

        if not os.path.exists(object_dir + "\\build\\windows"):
            os.makedirs(object_dir + "\\build\\windows")
        else:
            for file in os.listdir(object_dir + "\\build\\windows"):
                os.remove(os.path.join(object_dir + "\\build\\windows", file))

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

        subprocess.run(cmd, cwd=cpp_dir)
        if sys.argv[1] == "run":
            subprocess.run([exe_path], cwd=exe_dir)
    except Exception as e:
        print(e)
