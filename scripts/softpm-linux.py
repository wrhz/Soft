from enum import Enum
import os
import sys
import subprocess
import shutil
from print_data import print_data, Color

def get_package_name_from_dist(module):
    return module.split('-')[0]

def get_version(metadata_path):
    for line in metadata_path.splitlines():
        if line.startswith("Version:"):
            return line.split(":")[1].strip()
        
    return None

args = sys.argv[2:]
project_path = os.getcwd()
python_home = os.path.dirname(os.path.dirname(sys.executable))
packages_path = os.path.join(project_path, "packages", "linux")
mode = sys.argv[1]

try:
    if not os.path.exists(packages_path):
        os.makedirs(packages_path)

    match mode:
        case "install":
            print_data(f"Installing ...", Color.GREEN)
            subprocess.run(["pip", "install", "--isolated", "--disable-pip-version-check", *args, "--target", packages_path, "--break-system-packages"])

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
            with open(os.path.join(project_path, "softpm_requirements.txt"), "r") as f:
                for line in f.readlines():
                    line = line.strip()
                    if line and not line.startswith("#"):
                        package = ""
                        for char in line:
                            if char in ["=", "~", ">", "<", "^", "!", " "] or char.isspace():
                                break
                            package += char
                        print(package)

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
            subprocess.run(["pip", "install", "--isolated", "--disable-pip-version-check", "-r", os.path.join(project_path, "softpm_requirements.txt"), "--target", packages_path])

        case _:
            subprocess.run(["pip", *sys.argv[1:]])
    
    subprocess.run(["pip", "freeze", ">", "softpm_requirements.txt"], cwd=project_path)
except Exception as e:
    print_data(f"Error: {e}", Color.RED)
    sys.exit(1)
