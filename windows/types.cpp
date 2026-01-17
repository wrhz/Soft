#include "types.h"
#include <fstream>

namespace types {
    void init_element_struct(py::dict element, ElementStruct& element_struct)
    {
        for (auto item : element)
        {
            std::string key = item.first.cast<std::string>();
            py::handle value = item.second;
            if (key == "tag")
            {
                element_struct.tag = value.cast<std::string>();
            }
            else if (key == "text")
            {
                element_struct.text = value.cast<std::string>();
            }
            else if (key == "style")
            {
                for (auto style_item : value.attr("style"))
                {
                    std::string style_key = key;
                    std::string style_value = value.cast<std::string>();
                    element_struct.style[style_key] = style_value;
                }
            }
            else if (key == "children")
            {
                for (auto child : value)
                {
                    ElementStruct* child_struct = new ElementStruct();
                    init_element_struct(child.cast<py::dict>(), *child_struct);
                    element_struct.children.push_back(child_struct);
                }
            }
            std::ofstream file("soft.txt");
            file << py::str(value).cast<std::string>() << std::endl;
            file.close();
        }
    }

    void init_soft_struct(py::object soft, SoftStruct& soft_struct)
    {
        soft_struct.title = soft.attr("title").cast<std::wstring>();
        soft_struct.size = std::make_tuple(soft.attr("size").attr("width").cast<int>(), soft.attr("size").attr("height").cast<int>());
        soft_struct.home = new ElementStruct();
        init_element_struct(soft.attr("home").attr("element").cast<py::dict>(), *soft_struct.home);
    }
}