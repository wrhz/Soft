#include <pybind11/pybind11.h>
#include <register_modules.h>

PYBIND11_MODULE(init_extension, m)
{
    m.def("init", &initialize_python_module);

    m.def("uninit", &uninitialize_python_module);
}