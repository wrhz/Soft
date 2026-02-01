#ifndef SOFT_TYPES_H
#define SOFT_TYPES_H

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace soft::types {
    struct ElementStruct {
        std::string tag = "";
        std::string text = "";
        std::vector<ElementStruct*> children = {};
        std::map<std::string, std::string> style = {};
    };

    struct SoftStruct {
        std::string title = "Soft Windows";
        std::tuple<int, int> size = std::make_tuple(800, 600);
        ElementStruct* home = nullptr;
        std::vector<std::function<ElementStruct*()>> routes = {};
        std::string font_family = "Segoe UI Variable";
    };

    void init_soft_struct(py::object soft, SoftStruct& soft_struct);
    void init_element_struct(py::object element, ElementStruct& element_struct);
}

#endif // !SOFT_TYPES_H