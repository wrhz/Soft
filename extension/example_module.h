#ifndef EXAMPLE_MODULE_H
#define EXAMPLE_MODULE_H

#include <pybind11/pybind11.h>
#include <soft/module.h>
#include <string>

namespace py = pybind11;

class ExampleModule : public soft::SoftPythonModule
{

    public:
        void initialize(py::module& m) override;

        static std::string hello();

        std::string module_name = "example_module";
};

#endif