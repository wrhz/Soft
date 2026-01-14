#ifndef STYLE_H
#define STYLE_H

#include <windows.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace style {
    class Style {
    public:
        static UINT handle_style(HDC hdc, RECT rect, py::dict styles);
    };
}

#endif // STYLE_H