#include "types.h"
#include <vector>

namespace soft::types {
    Element::Element(py::object element)
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
                tag = value.cast<std::string>();
            }
            else if (key == "text")
            {
                text = value.cast<std::string>();
            }
            else if (key == "style")
            {
                for (auto style_item : value.cast<py::dict>())
                {
                    std::string style_key = style_item.first.cast<std::string>();
                    std::string style_value = style_item.second.cast<std::string>();
                    style[style_key] = style_value;
                }
            }
            else if (key == "children")
            {
                for (auto child : value)
                {
                    Element child_struct(child.cast<py::dict>());
                    children.push_back(&child_struct);
                }
            }
        }
    }

    Soft::Soft(py::object soft)
    {
        font_family = soft.attr("font_family").cast<std::string>();
        home = new Element(soft.attr("home")().attr("element"));
    }
}