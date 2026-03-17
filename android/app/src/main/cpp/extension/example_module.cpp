#include "example_module.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

void ExampleModule::initialize(py::module& m)
{
    m.def("hello", &ExampleModule::hello);
}

std::string ExampleModule::hello()
{
    return "Hello Soft Extension";
}
