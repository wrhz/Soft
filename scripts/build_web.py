import subprocess
import os
import shutil
import webbrowser
import json
import zipfile
from get_platform import get_platform
from build_modules import build_modules
from get_packages import get_packages_with_parser

def zip_folder(folder_path, output_path):
    with zipfile.ZipFile(output_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk(folder_path):
            rel_path = os.path.relpath(root, folder_path)
            if rel_path == '.':
                rel_path = ''
            
            for file in files:
                file_path = os.path.join(root, file)
                zip_path = os.path.join(rel_path, file)
                zipf.write(file_path, zip_path)

def build_web(project_dir):
    try:
        html_dir = os.path.join(project_dir, "build", "web")
        pyodide_path = os.path.join(project_dir, "python", "web-python.tar.bz2")
        platform = get_platform()

        if not os.path.exists(html_dir):
            os.makedirs(html_dir)
        else:
            for file in os.listdir(html_dir):
                if file not in ["pyodide"]:
                    if os.path.isdir(os.path.join(html_dir, file)):
                        shutil.rmtree(os.path.join(html_dir, file))
                    else:
                        os.remove(os.path.join(html_dir, file))

        if not os.path.exists(os.path.join(html_dir, "pyodide")):
            shutil.unpack_archive(pyodide_path, html_dir)

        for file in os.listdir(os.path.join(project_dir, "web")):
            if file in ["pyodide"]:
                continue

            if os.path.isdir(os.path.join(project_dir, "web", file)):
                shutil.copytree(os.path.join(project_dir, "web", file), os.path.join(html_dir, file))
            else:
                shutil.copy2(os.path.join(project_dir, "web", file), os.path.join(html_dir, file))

        if platform == "linux":
            python = "python3"
        else:
            python = "python"

        for dir in os.listdir(os.path.join(project_dir, "src", "python")):
            build_modules(os.path.join(project_dir, "src", "python", dir), os.path.join(html_dir, "src"), python)
        build_modules(os.path.join(project_dir, "lib"), html_dir, python)

        shutil.copytree(os.path.join(project_dir, "config"), os.path.join(html_dir, "config"))
        shutil.copytree(os.path.join(project_dir, "resources"), os.path.join(html_dir, "resources"))

        zip_folder(os.path.join(html_dir, "src"), os.path.join(html_dir, "src.zip"))
        zip_folder(os.path.join(html_dir, "lib"), os.path.join(html_dir, "lib.zip"))

        shutil.rmtree(os.path.join(html_dir, "src"))
        shutil.rmtree(os.path.join(html_dir, "lib"))
        
        soft_packages = set(get_packages_with_parser(os.path.join(project_dir, "softpm_requirements.txt")))
        pyodide_packages = set(json.load(open(os.path.join(html_dir, "pyodide", "pyodide-lock.json")))["packages"].keys())
        json.dump(list(soft_packages & pyodide_packages), open(os.path.join(html_dir, "packages.json"), "w"), indent=4)

        webbrowser.open(f"http://localhost:8000/index.html")
        subprocess.run(["python", "-m", "http.server"], cwd=html_dir)
    except Exception as e:
        print(f"Error: {e}")
