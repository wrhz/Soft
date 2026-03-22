#ifndef MODULE_H
#define MODULE_H

#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

namespace soft::module
{
    class SoftPythonModule
    {
        public:
        SoftPythonModule() = default;
        ~SoftPythonModule() = default;

        virtual void initialize(py::module& m) = 0;

        std::string module_name;
    };
}

#endif