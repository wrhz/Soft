#ifndef SOFT_TYPES_H
#define SOFT_TYPES_H

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <pybind11/embed.h>

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
    };

    class Soft {
    public:
        Soft() : home(nullptr), font_family("Segoe UI Variable") {}
        Soft(py::object soft);

        Element* home = nullptr;
        std::vector<std::function<Element*()>> routes = {};
        std::string font_family = "Segoe UI Variable";
    };
}

#endif // !SOFT_TYPES_H