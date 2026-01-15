import os
import sys
import subprocess
import platform

def build_linux(project_dir):
    try:
        x86_64_path = os.path.join(project_dir, "build", "linux", "main")
        x86_64_dir = os.path.dirname(x86_64_path)
        cpp_dir = os.path.join(project_dir, "linux")

        files = [
            "element.cpp",
            "style.cpp"
        ]

        includePaths = [
            f"{project_dir}/packages/pybind11/include/pybind11",
            f"/usr/include/python{platform.python_version().split('.')[0]}.{platform.python_version().split('.')[1]}"
        ]

        libPaths = [
            
        ]

        libFiles = [
            "cairo"
        ]

        other = [
            " -Wno-attributes",
            "$(python3-config --libs --embed)"
        ]

        cmd = [
            "g++",
            "-std=c++17",
            "-o",
            x86_64_path,
            "main.cpp",
        ]

        soft_linux_driver = os.environ.get("SOFT_LINUX_DRIVER", "X11")
        if soft_linux_driver.upper() == "X11":
            files.append("x11_driver.cpp")
            libFiles.append("X11")
            other.append("-DX11")
        elif soft_linux_driver.lower() == "wayland":
            files.append("wayland_driver.cpp")
            other.append("$(pkg-config --cflags --libs wayland-client++)")
        else:
            print(f"Invalid driver: {soft_linux_driver}")
            exit(1)

        if not os.path.exists(project_dir + "/build/linux"):
            os.makedirs(project_dir + "/build/linux")
        else:
            for file in os.listdir(project_dir + "/build/linux"):
                os.remove(os.path.join(project_dir + "/build/linux", file))

        if len(files) > 0:
            for file in files:
                cmd.append(file)
        if len(includePaths) > 0:
            for includePath in includePaths:
                cmd.append(f"-I{includePath}")
        if len(libPaths) > 0:
            for libPath in libPaths:
                cmd.append(f"-L{libPath}")
        if len(libFiles) > 0:
            for libFile in libFiles:
                cmd.append(f"-l{libFile}")
        if len(other) > 0:
            for otherFile in other:
                cmd.append(otherFile)

        os.chdir(cpp_dir)
        os.system(" ".join(cmd))
        if sys.argv[1] == "run":
            subprocess.run([x86_64_path, "--driver", soft_linux_driver], cwd=x86_64_dir)
    except Exception as e:
        print(f"Error: {e}")