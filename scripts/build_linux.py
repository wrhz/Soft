import os
import sys
import subprocess
import platform
import shutil
from get_files import get_files
from build_modules import build_modules
from write_cmake import write_cmake

def build_linux(project_dir):
    try:
        x86_64_path = os.path.join(project_dir, "build", "linux", "app")
        x86_64_dir = os.path.dirname(x86_64_path)
        cpp_dir = os.path.join(project_dir, "linux")

        files = {
            *get_files(os.path.join(project_dir, "linux"), remove_files=["main.cpp", "x11_driver.cpp", "wayland_driver.cpp"]),
            *get_files(os.path.join(project_dir, "src", "cpp")),
            *get_files(os.path.join(project_dir, "extension")),
        }

        public_include_paths = [
            os.path.join(project_dir, "src", "cpp", "public"),
            os.path.join(project_dir, "extension"),
        ]

        private_include_paths = [
            f"/usr/include/python{platform.python_version().split('.')[0]}.{platform.python_version().split('.')[1]}",
            "/usr/include/freetype2",
            os.path.join(project_dir, "src", "cpp", "private"),
        ]

        lib_paths = [
            "/usr/lib/x86_64-linux-gnu",
        ]

        lib_files = [
            "freetype",
            "cairo",
            "libpython3.12.so"
        ]

        defines = []

        soft_linux_driver = os.environ.get("SOFT_LINUX_DRIVER", "X11")
        if soft_linux_driver.upper() == "X11":
            files.add("linux/x11_driver.cpp")
            lib_files.append("X11")
            defines.append("X11")
        elif soft_linux_driver.lower() == "wayland":
            files.add("linux/wayland_driver.cpp")
        else:
            print(f"Invalid driver: {soft_linux_driver}")
            exit(1)

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

        with open(os.path.join(project_dir, "cmake", "linux.cmake"), "w") as f:
            write_cmake(f, files, private_include_paths, public_include_paths, lib_paths, lib_files, defines)

        subprocess.run(["cmake", "../..", "-G", "Ninja"], cwd=x86_64_dir)
        subprocess.run(["ninja"], cwd=x86_64_dir)

        if sys.argv[1] == "run":
            subprocess.run([x86_64_path], cwd=x86_64_dir)
    except Exception as e:
        print(f"Error: {e}")