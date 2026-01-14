import os

def build_module(venvs_dir: str, dir: str, project_dir: str):
    for file in os.listdir(dir):
        if os.path.isfile(os.path.join(dir, file)):
            file_path = os.path.join(dir, file)
            if file.endswith(".cpp"):
                os.system(f"{venvs_dir}/Scripts/python -m nuitka --module={file_path} --output-dir={dir}")
            else:
                with open(os.path.join(dir, file), "w") as wf:
                    with open(file_path, "r") as rf:
                        wf.write(rf.read())
        else:
            build_module(venvs_dir, os.path.join(dir, file), project_dir)
