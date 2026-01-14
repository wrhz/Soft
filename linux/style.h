#ifndef __STYLE_H__
#define __STYLE_H__

#include <pybind11/embed.h>
#include <tuple>

namespace py = pybind11;

namespace style {
    class Style
    {
    public:
        static std::tuple<int, int> handleStyle(int width, int height, int font_size, int text_width, int text_height, py::dict style);
    };
}

#endif