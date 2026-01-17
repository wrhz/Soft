import os

def get_files(project_dir: str, files: list[str] = [], remove_files: list[str] = []) -> list[str]:
    for file in os.listdir(project_dir):
        if file.endswith(".cpp") and file not in remove_files:
            files.append(os.path.join(project_dir, file))
        elif os.path.isdir(os.path.join(project_dir, file)):
            get_files(os.path.join(project_dir, file), files, remove_files)

    return files