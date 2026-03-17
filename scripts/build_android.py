import os
import sys
import subprocess
import shutil
import json
import xml.etree.ElementTree as ET
from build_modules import build_modules
from get_platform import get_platform

def build_android(project_dir):
    if get_platform() == "linux":
        python = "python3"
    else:
        python = "python"

    not_remove_files_and_dirs = [
        "init.py",
        "init_extension.cpp",
    ]

    android_python_dir = os.path.join(project_dir, "android", "app", "src", "main", "python")

    for file in os.listdir(android_python_dir):
        if file not in not_remove_files_and_dirs:
            if os.path.isfile(os.path.join(android_python_dir, file)):
                os.remove(os.path.join(android_python_dir, file))
            else:
                shutil.rmtree(os.path.join(android_python_dir, file))

    android_assets_dir = os.path.join(project_dir, "android", "app", "src", "main", "assets")

    for asset in os.listdir(android_assets_dir):
        if os.path.isfile(os.path.join(android_assets_dir, asset)):
            os.remove(os.path.join(android_assets_dir, asset))
        else:
            shutil.rmtree(os.path.join(android_assets_dir, asset))

    shutil.copytree(os.path.join(project_dir, "config"), os.path.join(android_assets_dir, "config"))

    if os.path.exists(os.path.join(project_dir, "android", "app", "src", "main", "cpp", "extension")):
        shutil.rmtree(os.path.join(project_dir, "android", "app", "src", "main", "cpp", "extension"))

    shutil.copytree(os.path.join(project_dir, "extension"), os.path.join(project_dir, "android", "app", "src", "main", "cpp", "extension"))
    for file in os.listdir(os.path.join(project_dir, "src", "cpp", "public")):
        file_path = os.path.join(project_dir, "src", "cpp", "public", file)
        output_path = os.path.join(project_dir, "android", "app", "src", "main", "cpp", file)

        if os.path.isdir(file_path):
            if os.path.exists(output_path):
                shutil.rmtree(output_path)

            shutil.copytree(file_path, output_path)
        else:
            shutil.copy(file_path, output_path)

    for resource in os.listdir(os.path.join(project_dir, "resources")):
        if os.path.isdir(os.path.join(project_dir, "resources", resource)):
            shutil.copytree(os.path.join(project_dir, "resources", resource), os.path.join(android_assets_dir, resource))
        else:
            shutil.copy(os.path.join(project_dir, "resources", resource), os.path.join(android_python_dir, resource))

    build_modules(os.path.join(project_dir, "src", "python"), os.path.dirname(android_python_dir), python, False)
    build_modules(os.path.join(project_dir, "lib"), android_python_dir, python, False)

    with open(os.path.join(project_dir, "config", "soft.json"), "r") as f:
        soft_config = json.load(f)

    with open(os.path.join(project_dir, "android", "app", "src", "main", "res", "values", "strings.xml"), "r") as f:
        tree = ET.parse(f)
        root = tree.getroot()
        target = root.find('.//*[@name="app_name"]')
        if target is not None:
            target.text = soft_config["title"]
        
    tree.write(os.path.join(project_dir, "android", "app", "src", "main", "res", "values", "strings.xml"), encoding="utf-8", xml_declaration=True)

    if sys.argv[1] == "run":
        subprocess.run(["gradlew.bat", "installDebug"], cwd=os.path.join(project_dir, "android"), shell=True)
    else:
        subprocess.run(["gradlew.bat", "assembleRelease"], cwd=os.path.join(project_dir, "android"), shell=True)
