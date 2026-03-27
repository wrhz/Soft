#ifndef SOFT_TYPES_H
#define SOFT_TYPES_H

#include "yoga/Yoga.h"
#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <pybind11/embed.h>

#ifdef __linux__
#include <cairo/cairo.h>
#else _WIN32
#include <windows.h>
#endif

namespace py = pybind11;

namespace soft::types {
    class Element {
    public:
        Element() : tag(""), text(""), children({}), style({}) {}
        Element(py::object element);
        
        std::string tag = "";
        std::string text = "";
        std::vector<Element*> children = {};
        std::map<std::string, std::string> style = {};
        YGNodeRef node = nullptr;
#ifdef __linux__
        void (*render)(cairo_t *cr, Element& element) = nullptr;
#elif _WIN32
        void (*render)(HDC hdc, Element& element) = nullptr;
#endif
    };

    class Soft {
    public:
        Soft() : home(), default_font_family("Arial") {}
        Soft(py::object soft);

        py::object home;
        std::vector<std::function<Element*()>> routes = {};
        std::string default_font_family = "Arial";
    };
}

#endif // !SOFT_TYPES_H