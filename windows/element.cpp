#include "element.h"
#include "style.h"

void element::Element::draw(py::dict root_element, HDC hdc, RECT rect)
{
    py::object style_object = root_element["style"];
    style::StyleStruct style;
    if (!style_object.is(py::none()))
    {
        style = style::Style::handle_style(hdc, rect, style_object.attr("style"));
    }
    std::string tag = root_element["tag"].cast<std::string>();
    if (tag == "text")
    {
        std::wstring text = root_element["text"].cast<std::wstring>();
        DrawTextW(hdc, text.c_str(), -1, &rect, style.format_style);
    }
}