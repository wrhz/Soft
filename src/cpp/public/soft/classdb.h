#ifndef CLASSDB_H
#define CLASSDB_H

#include "module.h"

namespace soft
{
    class ClassDB
    {
    private:
        static py::module::module_def static_moddef;

    public:
        template <typename T>
        static void register_class()
        {
            T Class = T();

            std::string module_name = Class.module_name;

            py::module m = py::module::create_extension_module(
                module_name.c_str(),
                "",
                &static_moddef
            );

            py::module::import("sys").attr("modules")[module_name.c_str()] = m;

            Class.initialize(m);
        }
    };
}

#endif