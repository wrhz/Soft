from enum import Enum
import os
import sys
import subprocess
import shutil

class Color(Enum):
    RED = "31"
    GREEN = "32"
    YELLOW = "33"
    BLUE = "34"
    MAGENTA = "35"
    CYAN = "36"
    WHITE = "37"

def print_data(data, color: Color):
    print(f"\033[{color.value}m{data}\033[0m")

def get_package_name_from_dist(module):
    return module.split('-')[0]

def get_version(metadata_path):
    for line in metadata_path.splitlines():
        if line.startswith("Version:"):
            return line.split(":")[1].strip()
        
    return None

def save_lock_json():
    lock_data = ""

    for file in os.listdir(packages_path):
        if file.endswith("dist-info"):
            module = get_package_name_from_dist(file)
            version = get_version(open(os.path.join(packages_path, file, "METADATA")).read())
            if version is None:
                print_data(f"Could not get version for {module}", Color.RED)
                continue

            lock_data += f"{module}=={version}\n"

    with open(os.path.join(project_path, "softpm_requirements.txt"), "w") as f:
        f.write(lock_data)

if sys.platform == "win32":
    platform = "windows"
elif sys.platform == "linux":
    platform = "linux"
else:
    print_data(f"Invalid platform: {sys.platform}", Color.RED)
    exit(1)

args = sys.argv[2:]
project_path = os.path.dirname(os.path.abspath(__file__))
python_home = os.path.join(project_path, "build", platform, "python")
pip_path = os.path.join(python_home, "Scripts", "pip.exe")
packages_path = os.path.join(project_path, "packages")
mode = sys.argv[1]

try:
    match mode:
        case "install":
            print_data(f"Installing ...", Color.GREEN)
            subprocess.run([pip_path, "install", "--isolated", "--disable-pip-version-check", *args, "--target", packages_path])

        case "uninstall":
            print_data(f"Uninstalling ...", Color.RED)
            for module in os.listdir(packages_path):
                if get_package_name_from_dist(module).lower() in args:      
                    shutil.rmtree(os.path.join(packages_path, module), ignore_errors=False)
                    if not module.endswith(".dist-info"):
                        print_data(f"Uninstalled {module}", Color.RED)
                elif module.split(".")[0].lower() in args and os.path.isfile(os.path.join(packages_path, module)):
                    os.remove(os.path.join(packages_path, module))

            if len(os.listdir(packages_path)) == 1:
                print_data(f"No modules installed", Color.YELLOW)
                shutil.rmtree(os.path.join(packages_path, "bin"), ignore_errors=False)

        case "list":
            print_data(f"Listing installed modules ...", Color.YELLOW)
            for module in os.listdir(packages_path):
                if module != "bin" and module != "__pycache__" and not module[-9:].endswith("dist-info"):
                    print(module)

        case "clean":
            print_data(f"Cleaning ...", Color.YELLOW)
            try:
                shutil.rmtree(packages_path, ignore_errors=False)
            except FileNotFoundError:
                pass
            except Exception as e:
                print_data(f"Failed to delete {packages_path}: {e}", Color.RED)
            else:
                print_data(f"Cleaned {packages_path}", Color.GREEN)

            os.makedirs(packages_path)

        case "sync":
            print_data(f"Syncing ...", Color.CYAN)
            subprocess.run([pip_path, "install", "--isolated", "--disable-pip-version-check", "-r", os.path.join(project_path, "softpm_requirements.txt"), "--target", packages_path])

        case _:
            subprocess.run([pip_path, *sys.argv[1:]])
    
    save_lock_json()
except Exception as e:
    print_data(f"Error: {e}", Color.RED)
    sys.exit(1)
