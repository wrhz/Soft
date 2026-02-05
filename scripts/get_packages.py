import requirements

def get_packages_with_parser(file_path):
    packages = []
    with open(file_path, 'r', encoding='utf-8') as f:
        for req in requirements.parse(f):
            if req.name:
                packages.append(req.name)
    return packages
