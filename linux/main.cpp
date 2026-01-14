#include <pybind11/embed.h>
#ifdef X11
#include "x11_driver.h"
#endif

#ifdef WAYLAND
#include "wayland_driver"
#endif

#include <filesystem>
#include <iostream>

namespace py = pybind11;
namespace fs = std::filesystem;

int use_driver = 1;

void init()
{
    fs::path current_path = fs::current_path();
    
    fs::path project_root = current_path.parent_path().parent_path();
    
    py::module_ sys = py::module_::import("sys");
    
    sys.attr("path").attr("append")(project_root.string());
    
    fs::path lib_path = project_root / "lib";
    sys.attr("path").attr("append")(lib_path.string());

    fs::path src_path = project_root / "src";
    sys.attr("path").attr("append")(src_path.string());
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
            int result = run_driver->init(mainSoft);
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