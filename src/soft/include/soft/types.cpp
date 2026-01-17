#include "types.h"

namespace soft::types {
    void init_element_struct(py::object element, ElementStruct& element_struct)
    {
        py::dict element_dict;
        
        if (py::isinstance<py::dict>(element)) {
            element_dict = element.cast<py::dict>();
        } else {
            element_dict = element.attr("__dict__");
        }

        for (auto item : element_dict)
        {
            std::string key = item.first.cast<std::string>();
            py::handle value = item.second;
            if (key == "tag")
            {
                element_struct.tag = value.cast<std::string>();
            }
            else if (key == "text")
            {
                element_struct.text = value.cast<std::wstring>();
            }
            else if (key == "style")
            {
                for (auto style_item : value.cast<py::dict>())
                {
                    std::string style_key = style_item.first.cast<std::string>();
                    std::string style_value = style_item.second.cast<std::string>();
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
        }
    }

    void init_soft_struct(py::object soft, SoftStruct& soft_struct)
    {
        soft_struct.title = soft.attr("title").cast<std::wstring>();
        soft_struct.size = soft.attr("size").cast<std::tuple<int, int>>();
        soft_struct.home = new ElementStruct();
        init_element_struct(soft.attr("home")().attr("element"), *soft_struct.home);
    }
}