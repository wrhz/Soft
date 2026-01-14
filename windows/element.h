#ifndef ELEMENT_H
#define ELEMENT_H

#include <windows.h>
#include <pybind11/pybind11.h>
#include "style.h"

namespace py = pybind11;

namespace element {
    class Element {
    public:
        static void draw(py::dict root_element, HDC hdc, RECT rect);
    };
}

#endif // ELEMENT_H