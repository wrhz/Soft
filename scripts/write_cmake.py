def write_cmake(f, files, private_include_paths, public_include_paths, lib_paths, lib_files, defines):
    f.write("set(APP_SOURCES\n")

    for file in files:
        f.write(f"    \"{file}\"\n".replace("\\", "/"))

    f.write(")\n")

    f.write("target_include_directories(app\n")
    
    f.write("    PRIVATE\n")

    for include_path in private_include_paths:
        f.write(f"    \"{include_path}\"\n".replace("\\", "/"))
    
    f.write("    PUBLIC\n")

    for include_path in public_include_paths:
        f.write(f"    \"{include_path}\"\n".replace("\\", "/"))

    f.write(")\n")

    for lib_path in lib_paths:
        f.write(f"target_link_directories(app PRIVATE \"{lib_path}\")\n".replace("\\", "/"))

    for lib_file in lib_files:
        f.write(f"target_link_libraries(app PRIVATE \"{lib_file}\")\n".replace("\\", "/"))

    for define in defines:
        f.write(f"target_compile_definitions(app PRIVATE \"{define}\")\n".replace("\\", "/"))