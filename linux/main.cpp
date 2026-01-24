#include <pybind11/embed.h>
#ifdef X11
#include "x11_driver.h"
#endif

#ifdef WAYLAND
#include "wayland_driver"
#endif

#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <libgen.h>

namespace py = pybind11;
namespace fs = std::filesystem;

int use_driver = 1;

std::string get_exe_parent_dir()
{
    char exe_path[PATH_MAX];
    char *parent_dir;

    ssize_t count = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (count == -1) {
        perror("readlink failed");
        return "";
    }
    exe_path[count] = '\0';

    char path_copy[PATH_MAX];
    strncpy(path_copy, exe_path, sizeof(path_copy));
    parent_dir = dirname(path_copy);

    char parent_dir_saved[PATH_MAX];
    strncpy(parent_dir_saved, parent_dir, sizeof(parent_dir_saved) - 1);
    parent_dir_saved[sizeof(parent_dir_saved) - 1] = '\0';

    return parent_dir_saved;
}

void init()
{
    py::module_ sys = py::module_::import("sys");

    fs::path exe_dir = get_exe_parent_dir();
    
    fs::path lib_path = exe_dir / "lib";
    sys.attr("path").attr("append")(lib_path.string());

    fs::path src_path = exe_dir / "src";
    sys.attr("path").attr("append")(src_path.string());

    fs::path packages_path = exe_dir / "packages";
    sys.attr("path").attr("append")(packages_path.string());
}

std::string lower(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if ('A' <= str[i] && str[i] <= 'Z')
        {
            str[i] = str[i] + 32;
        }
    }

    return str;
}

int handle_args(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "--driver")
        {
            if (i + 1 >= argc) {
                std::cerr << "--driver 选项需要参数" << std::endl;
                return 1;
            }
            
            std::string driver = std::string(argv[++i]);
            if (lower(driver) == "wayland")
            {
                use_driver = 2;
            }
            else if (lower(driver) != "x11")
            {
                std::cerr << "Not found driver " << driver << std::endl;
                return 1;
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if (handle_args(argc, argv) != 0)
    {
        return 1;
    }

    try {
        py::scoped_interpreter guard{};

        init();

        py::module_ main_module = py::module_::import("lib.main");
        py::object mainSoft = main_module.attr("main")();

        if (use_driver == 1)
        {
            auto run_driver = std::make_unique<driver::X11Driver>();
            soft::types::SoftStruct main_soft_struct;
            soft::types::init_soft_struct(mainSoft, main_soft_struct);
            int result = run_driver->init(main_soft_struct);
            if (result != 0)
            {
                std::cerr << "驱动初始化失败: " << result << std::endl;
                return 1;
            }

            run_driver->run();
            run_driver->finish();
        }
        else
        {
            std::cerr << "Not found driver to using" << std::endl;
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}