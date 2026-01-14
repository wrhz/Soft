import os
import sys
import subprocess
import platform
# from extract_zip import extract_zip

def build_windows(python_home, project_dir):
    try:
        exe_path = os.path.join(project_dir, "build", "windows", "main.exe")
        exe_dir = os.path.dirname(exe_path)
        cpp_dir = os.path.join(project_dir, "windows")

        files = [
            "element.cpp",
            "style.cpp"
        ]

        includePaths = [
            f"{project_dir}\\packages\\pybind11\\include",
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
            f"/Fe:{os.path.join(project_dir, "build", "windows", "main.exe")}",
            f"/Fd:{os.path.join(project_dir, "build", "windows", "main.pdb")}",
            "main.cpp"
        ]

        if not os.path.exists(project_dir + "\\build\\windows"):
            os.makedirs(project_dir + "\\build\\windows")

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

        # extract_zip(os.path.join(project_dir, "python-embed", "python-3.12.9-embed-amd64.zip"), os.path.join(exe_dir, "python312"))

        subprocess.run(cmd, cwd=cpp_dir)
        if sys.argv[1] == "run":
            subprocess.run([exe_path], cwd=exe_dir)
    except Exception as e:
        print(e)