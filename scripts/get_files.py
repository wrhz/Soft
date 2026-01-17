import os

def get_files(project_dir: str, files: list[str] = []) -> list[str]:
    for file in os.listdir(project_dir):
        if file.endswith(".cpp"):
            files.append(os.path.join(project_dir, file))
        elif os.path.isdir(os.path.join(project_dir, file)):
            get_files(os.path.join(project_dir, file), files)

    return files