#ifndef STYLE_H
#define STYLE_H

#include <windows.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace style {
    struct StyleStruct
    {
        UINT format_style = DT_SINGLELINE;
    };

    class Style {
    public:
        static StyleStruct handle_style(HDC hdc, RECT rect, py::dict styles);
    };
}

#endif // STYLE_H